#define STB_IMAGE_IMPLEMENTATION

#include "../../../stb/include/stb_image.h"
#include "../../include/wavefront/wavefront.h"

#define STENT_DISABLE
#define STENT_IMPLEMENTATION
#include "stent.h"

struct Vec2
{
  float x;
  float y;
};

struct Vec3
{
  float x;
  float y;
  float z;
};

struct Vec4
{
  float x;
  float y;
  float z;
  float w;
};

struct Vertex
{
  struct Vec3 position;
  struct Vec3 normal;
  struct Vec2 texCoord;
};

struct Face
{
  struct Vertex a;
  struct Vertex b;
  struct Vertex c;
};

struct Material
{
  ref(sstream) name;
  ref(sstream) texture;
};

static void
vector_sstream_delete(vector(ref(sstream)) ctx)
{
  size_t i = 0;

  for(i = 0; i < vector_size(ctx); i++)
  {
    sstream_delete(vector_at(ctx, i));
  }

  vector_delete(ctx);
}

static void
SplitString(ref(sstream) ss, char splitter, vector(ref(sstream)) out)
{
  size_t max = 0;
  size_t i = 0;
  ref(sstream) curr = NULL;
  char ch = 0;

  /*
   * If out array already larger than 0, use that existing string
   * after blanking it.
   */
  if(max >= vector_size(out))
  {
    curr = sstream_new();
  }
  else
  {
    curr = vector_at(out, max);
    sstream_str_cstr(curr, "");
  }

  for(i = 0; i < sstream_length(ss); i++)
  {
    ch = sstream_at(ss, i);

    if(ch == splitter)
    {
      /*
       * String is already in array if max is still smaller than array
       */
      if(max >= vector_size(out))
      {
        vector_push_back(out, curr);
      }
      max++;

      /*
       * If out array already larger than max, use that existing string
       * after blanking it.
       */
      if(max >= vector_size(out))
      {
        curr = sstream_new();
      }
      else
      {
        curr = vector_at(out, max);
        sstream_str_cstr(curr, "");
      }
    }
    else
    {
      sstream_append_char(curr, ch);
    }
  }

  /*
   * If remaining curr is not blank. Add it to out array
   * if not already reusing an element.
   */
  if(sstream_length(curr) > 0)
  {
    if(max >= vector_size(out))
    {
      vector_push_back(out, curr);
    }
    max++;
  }
  else
  {
    if(max >= vector_size(out))
    {
      sstream_delete(curr);
    }
  }

  /*
   * Erase the remaining elements. They are not needed.
   */
  while(vector_size(out) > max)
  {
    sstream_delete(vector_at(out, vector_size(out) - 1));
    vector_erase(out, vector_size(out) - 1, 1);
  }
}

static void
SplitStringWhitespace(ref(sstream) ss, vector(ref(sstream)) out)
{
  size_t max = 0;
  size_t i = 0;
  ref(sstream) curr = NULL;
  char ch = 0;

  /*
   * If out array already larger than 0, use that existing string
   * after blanking it.
   */
  if(max >= vector_size(out))
  {
    curr = sstream_new();
  }
  else
  {
    curr = vector_at(out, max);
    sstream_str_cstr(curr, "");
  }

  for(i = 0; i < sstream_length(ss); i++)
  {
    ch = sstream_at(ss, i);

    if(ch == ' ' || ch == '\t')
    {
      /*
       * Add to out array if not an empty token
       */
      if(sstream_length(curr) > 0)
      {
        /*
         * String is already in array if max is still smaller than array
         */
        if(max >= vector_size(out))
        {
          vector_push_back(out, curr);
        }
        max++;

        /*
         * If out array already larger than max, use that existing string
         * after blanking it.
         */
        if(max >= vector_size(out))
        {
          curr = sstream_new();
        }
        else
        {
          curr = vector_at(out, max);
          sstream_str_cstr(curr, "");
        }
      }
    }
    else
    {
      sstream_append_char(curr, ch);
    }
  }

  /*
   * If remaining curr is not blank. Add it to out array
   * if not already reusing an element.
   */
  if(sstream_length(curr) > 0)
  {
    if(max >= vector_size(out))
    {
      vector_push_back(out, curr);
    }
    max++;
  }
  else
  {
    if(max >= vector_size(out))
    {
      sstream_delete(curr);
    }
  }

  /*
   * Erase the remaining elements. They are not needed.
   */
  while(vector_size(out) > max)
  {
    sstream_delete(vector_at(out, vector_size(out) - 1));
    vector_erase(out, vector_size(out) - 1, 1);
  }
}

static void
FindPathTrunk(ref(sstream) path)
{
  int ci = sstream_length(path) - 1;

  for(; ci >= 0; ci--)
  {
    char c = sstream_at(path, ci);

    if(c == '/' || c == '\\')
    {
      break;
    }
  }

  if(ci == -1)
  {
    sstream_str_cstr(path, "");
  }
  else
  {
    sstream_erase(path, ci, sstream_length(path) - ci);
  }
}

static void
ProcessMtl(ref(sstream) trunk, ref(sstream) mtlFile,
  vector(struct Material) materials)
{
  ref(sstream) path = sstream_new_str(trunk);
  sstream_append_char(path, '/');
  sstream_append(path, mtlFile);

  ref(ifstream) file = ifstream_open(path);
  sstream_delete(path);
  if(!file) return;

  ref(sstream) line = sstream_new();
  vector(ref(sstream)) tokens = vector_new(ref(sstream));

  struct Material m = {0};

  while(!ifstream_eof(file))
  {
    ifstream_getline(file, line);
    if(sstream_length(line) < 1) continue;
    SplitStringWhitespace(line, tokens);
    if(vector_size(tokens) < 1) continue;

    if(strcmp(sstream_cstr(vector_at(tokens, 0)), "newmtl") == 0)
    {
      if(vector_size(tokens) < 2) continue;
      m.name = sstream_new_str(vector_at(tokens, 1));
      m.texture = sstream_new();
      vector_push(materials, m);
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "map_Kd") == 0)
    {
      if(vector_size(tokens) < 2) continue;
      if(!m.name) m.name = sstream_new_cstr("Untitled");
      if(!m.texture) m.texture = sstream_new();
      sstream_append(m.texture, trunk);
      sstream_append_char(m.texture, '/');
      sstream_append(m.texture, vector_at(tokens, 1));

      if(vector_size(materials) < 1)
      {
        vector_push(materials, m);
      }
      else
      {
        vector_at(materials, vector_size(materials) - 1) = m;
      }
    }
  }

  vector_sstream_delete(tokens);
  sstream_delete(line);
  ifstream_close(file);
}

int
WfModelLoad(const char *path, struct WfModel *ctx)
{
  ref(ifstream) file = ifstream_open_cstr(path);
  if(!file) return 1;

  vector(struct Vec3) positions = vector_new(struct Vec3);
  vector(struct Vec2) texcoords = vector_new(struct Vec2);
  vector(struct Vec3) normals = vector_new(struct Vec3);
  vector(struct Face) faces = vector_new(struct Face);

  ref(sstream) line = sstream_new();
  vector(ref(sstream)) tokens = vector_new(ref(sstream));
  vector(ref(sstream)) sub = vector_new(ref(sstream));
  ref(sstream) filePath = sstream_new_cstr(path);
  vector(struct Material) materials = vector_new(struct Material);

  FindPathTrunk(filePath);

  while(!ifstream_eof(file))
  {
    ifstream_getline(file, line);
    if(sstream_length(line) < 1) continue;
    SplitStringWhitespace(line, tokens);
    if(vector_size(tokens) < 1) continue;

    if(strcmp(sstream_cstr(vector_at(tokens, 0)), "v") == 0)
    {
      while(vector_size(tokens) < 4) vector_push(tokens, sstream_new_cstr("0"));

      struct Vec3 val = {
        atof(sstream_cstr(vector_at(tokens, 1))),
        atof(sstream_cstr(vector_at(tokens, 2))),
        atof(sstream_cstr(vector_at(tokens, 3)))};

      vector_push_back(positions, val);
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "vn") == 0)
    {
      while(vector_size(tokens) < 4) vector_push(tokens, sstream_new_cstr("0"));

      struct Vec3 val = {
        atof(sstream_cstr(vector_at(tokens, 1))),
        atof(sstream_cstr(vector_at(tokens, 2))),
        atof(sstream_cstr(vector_at(tokens, 3)))};

      vector_push_back(normals, val);
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "vt") == 0)
    {
      while(vector_size(tokens) < 3) vector_push(tokens, sstream_new_cstr("0"));

      struct Vec2 val = {
        atof(sstream_cstr(vector_at(tokens, 1))),
        1.0f - atof(sstream_cstr(vector_at(tokens, 2)))};

      vector_push_back(texcoords, val);
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "f") == 0)
    {
      ref(MaterialGroup) mg = NULL;
      struct Face f = {0};
      struct Face qf = {0};

#define SUBSPLIT(I, D, S)                              \
  if(vector_size(sub) > I)                             \
  {                                                    \
    unsigned int i = atoi(sstream_cstr(vector_at(sub, I))) - 1; \
    if(i < 0 || i >= vector_size(S)) continue;         \
    D = vector_at(S, i);                               \
  }

      SplitString(vector_at(tokens, 1), '/', sub);
      SUBSPLIT(0, f.a.position, positions)
      SUBSPLIT(1, f.a.texCoord, texcoords)
      SUBSPLIT(2, f.a.normal, normals)

      SplitString(vector_at(tokens, 2), '/', sub);
      SUBSPLIT(0, f.b.position, positions)
      SUBSPLIT(1, f.b.texCoord, texcoords)
      SUBSPLIT(2, f.b.normal, normals)

      SplitString(vector_at(tokens, 3), '/', sub);
      SUBSPLIT(0, f.c.position, positions)
      SUBSPLIT(1, f.c.texCoord, texcoords)
      SUBSPLIT(2, f.c.normal, normals)

      vector_push(faces, f);

      if(vector_size(tokens) < 5) continue;

      qf.a.position = f.c.position;
      qf.a.texCoord = f.c.texCoord;
      qf.a.normal = f.c.normal;

      SplitString(vector_at(tokens, 4), '/', sub);
      SUBSPLIT(0, qf.b.position, positions)
      SUBSPLIT(1, qf.b.texCoord, texcoords)
      SUBSPLIT(2, qf.b.normal, normals)

      qf.c.position = f.a.position;
      qf.c.texCoord = f.a.texCoord;
      qf.c.normal = f.a.normal;

      vector_push(faces, qf);
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "usemtl") == 0)
    {
      /*TODO*/
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "mtllib") == 0)
    {
      if(vector_size(tokens) < 2) continue;
      ProcessMtl(filePath, vector_at(tokens, 1), materials);
    }
  }

  sstream_delete(filePath);
  ifstream_close(file);
  vector_delete(normals);
  vector_delete(positions);
  vector_delete(texcoords);
  vector_sstream_delete(sub);
  vector_sstream_delete(tokens);
  sstream_delete(line);

  if(!glGenBuffers)
  {
    panic("glGenBuffers not functional");
  }

  vector(float) buffer = vector_new(float);
  struct WfModel rtn = {0};
  rtn.vertexCount = vector_size(faces) * 3;
  glGenVertexArrays(1, &rtn.vaoId);
  glBindVertexArray(rtn.vaoId);

  glGenBuffers(1, &rtn.positionVboId);

  if(rtn.positionVboId)
  {
    foreach(struct Face face, faces,
      vector_push(buffer, face.a.position.x);
      vector_push(buffer, face.a.position.y);
      vector_push(buffer, face.a.position.z);

      vector_push(buffer, face.b.position.x);
      vector_push(buffer, face.b.position.y);
      vector_push(buffer, face.b.position.z);

      vector_push(buffer, face.c.position.x);
      vector_push(buffer, face.c.position.y);
      vector_push(buffer, face.c.position.z);
    )

    glBindBuffer(GL_ARRAY_BUFFER, rtn.positionVboId);

    glBufferData(GL_ARRAY_BUFFER, vector_size(buffer) * sizeof(vector_at(buffer, 0)),
      &vector_at(buffer, 0), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);
    vector_clear(buffer);
  }

  glGenBuffers(1, &rtn.normalVboId);

  if(rtn.normalVboId)
  {
    foreach(struct Face face, faces,
      vector_push(buffer, face.a.normal.x);
      vector_push(buffer, face.a.normal.y);
      vector_push(buffer, face.a.normal.z);

      vector_push(buffer, face.b.normal.x);
      vector_push(buffer, face.b.normal.y);
      vector_push(buffer, face.b.normal.z);

      vector_push(buffer, face.c.normal.x);
      vector_push(buffer, face.c.normal.y);
      vector_push(buffer, face.c.normal.z);
    )

    glBindBuffer(GL_ARRAY_BUFFER, rtn.normalVboId);

    glBufferData(GL_ARRAY_BUFFER, vector_size(buffer) * sizeof(vector_at(buffer, 0)),
      &vector_at(buffer, 0), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(2);
    vector_clear(buffer);
  }

  glGenBuffers(1, &rtn.texCoordVboId);

  if(rtn.texCoordVboId)
  {
    foreach(struct Face face, faces,
      vector_push(buffer, face.a.texCoord.x);
      vector_push(buffer, face.a.texCoord.y);

      vector_push(buffer, face.b.texCoord.x);
      vector_push(buffer, face.b.texCoord.y);

      vector_push(buffer, face.c.texCoord.x);
      vector_push(buffer, face.c.texCoord.y);
    )

    glBindBuffer(GL_ARRAY_BUFFER, rtn.texCoordVboId);

    glBufferData(GL_ARRAY_BUFFER, vector_size(buffer) * sizeof(vector_at(buffer, 0)),
      &vector_at(buffer, 0), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  vector_delete(faces);
  vector_delete(buffer);

  if(vector_size(materials) > 0)
  {
    struct Material m = vector_at(materials, 0);

    int w = 0;
    int h = 0;
    int channels = 0;

    unsigned char *data = stbi_load(sstream_cstr(m.texture), &w, &h, &channels, 4);

    if(data)
    {
      glGenTextures(1, &rtn.textureId);

      if(rtn.textureId)
      {
        glBindTexture(GL_TEXTURE_2D, rtn.textureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
      }

      free(data);
    }
  }

  foreach(struct Material m, materials,
    sstream_delete(m.name);
    sstream_delete(m.texture);
  )

  vector_delete(materials);

  *ctx = rtn;

  return 0;
}

void
WfModelDestroy(struct WfModel *ctx)
{
  if(ctx->positionVboId) glDeleteBuffers(1, &ctx->positionVboId);
  if(ctx->texCoordVboId) glDeleteBuffers(1, &ctx->texCoordVboId);
  if(ctx->normalVboId) glDeleteBuffers(1, &ctx->normalVboId);
  if(ctx->vaoId) glDeleteVertexArrays(1, &ctx->vaoId);
  if(ctx->textureId) glDeleteTextures(1, &ctx->textureId);
}