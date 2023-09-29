#include <GL/glew.h>

struct WfModel
{
  GLuint positionVboId; /* VBO containing the positions */
  GLuint texCoordVboId; /* VBO containing the texture coordinates */
  GLuint normalVboId;   /* VBO containing the normals */

  GLuint vaoId;         /* VAO with the above in streams 0, 1, 2 respectively */
  GLuint textureId;     /* First texture utilized by the model */
  int vertexCount;      /* Number of vertices within the model */
};

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************
 * WfModelLoad
 *
 * Load a model file into an OpenGL compatible structure, including the
 * VBOs, VAOs, texture ID and number of vertices.
 *
 * path: The file path from where to load the model
 *  ctx: The output structure to populate with loaded data
 *
 * return: 0 on success otherwise the error specific code.
 ***************************************************************************/
int
WfModelLoad(const char *path, struct WfModel *ctx);

/***************************************************************************
 * WfModelDestroy
 *
 * Deallocate all the OpenGL structures within the model data.
 *
 * ctx: The structure to deallocate
 ***************************************************************************/
void
WfModelDestroy(struct WfModel *ctx);

#ifdef __cplusplus
}
#endif

