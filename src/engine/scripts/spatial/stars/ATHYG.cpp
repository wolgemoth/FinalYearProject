#include "ATHYG.h"

#include "../../core/utils/Utils.h"

#include <array>
#include <cstddef>
#include <string>

namespace LouiEriksson::Engine::Spatial {
	
	ATHYG::V1::V1(const std::array<std::string, 23>& _values) {
		
		Utils::TryParse<size_t       >(_values[ 0], id      );
		Utils::TryParse<std::string  >(_values[ 1], tyc     );
		Utils::TryParse<size_t       >(_values[ 2], gaia    );
		Utils::TryParse<size_t       >(_values[ 3], hyg     );
		Utils::TryParse<size_t       >(_values[ 4], hip     );
		Utils::TryParse<size_t       >(_values[ 5], hd      );
		Utils::TryParse<size_t       >(_values[ 6], hr      );
		Utils::TryParse<std::string  >(_values[ 7], gl      );
		Utils::TryParse<std::string  >(_values[ 8], bayer   );
		Utils::TryParse<std::string  >(_values[ 9], flam    );
		Utils::TryParse<std::string  >(_values[10], con     );
		Utils::TryParse<std::string  >(_values[11], proper  );
		Utils::TryParse<double       >(_values[12], ra      );
		Utils::TryParse<double       >(_values[13], dec     );
		Utils::TryParse<std::string  >(_values[14], pos_src );
		Utils::TryParse<double       >(_values[15], dist    );
		Utils::TryParse<double       >(_values[16], x0      );
		Utils::TryParse<double       >(_values[17], y0      );
		Utils::TryParse<double       >(_values[18], z0      );
		Utils::TryParse<std::string  >(_values[19], dist_src);
		Utils::TryParse<double       >(_values[20], mag     );
		Utils::TryParse<double       >(_values[21], absmag  );
		Utils::TryParse<std::string  >(_values[22], mag_src );
	}
	
	ATHYG::V2::V2(const std::array<std::string, 33>& _values) {
		
		Utils::TryParse<size_t       >(_values[ 0], id       );
		Utils::TryParse<std::string  >(_values[ 1], tyc      );
		Utils::TryParse<size_t       >(_values[ 2], gaia     );
		Utils::TryParse<size_t       >(_values[ 3], hyg      );
		Utils::TryParse<size_t       >(_values[ 4], hip      );
		Utils::TryParse<size_t       >(_values[ 5], hd       );
		Utils::TryParse<size_t       >(_values[ 6], hr       );
		Utils::TryParse<std::string  >(_values[ 7], gl       );
		Utils::TryParse<std::string  >(_values[ 8], bayer    );
		Utils::TryParse<std::string  >(_values[ 9], flam     );
		Utils::TryParse<std::string  >(_values[10], con      );
		Utils::TryParse<std::string  >(_values[11], proper   );
		Utils::TryParse<double       >(_values[12], ra       );
		Utils::TryParse<double       >(_values[13], dec      );
		Utils::TryParse<std::string  >(_values[14], pos_src  );
		Utils::TryParse<double       >(_values[15], dist     );
		Utils::TryParse<double       >(_values[16], x0       );
		Utils::TryParse<double       >(_values[17], y0       );
		Utils::TryParse<double       >(_values[18], z0       );
		Utils::TryParse<std::string  >(_values[19], dist_src );
		Utils::TryParse<double       >(_values[20], mag      );
		Utils::TryParse<double       >(_values[21], absmag   );
		Utils::TryParse<std::string  >(_values[22], mag_src  );
		Utils::TryParse<double       >(_values[23], rv       );
		Utils::TryParse<std::string  >(_values[24], rv_src   );
		Utils::TryParse<double       >(_values[25], pm_ra    );
		Utils::TryParse<double       >(_values[26], pm_dec   );
		Utils::TryParse<double       >(_values[27], pm_src   );
		Utils::TryParse<double       >(_values[28], vx       );
		Utils::TryParse<double       >(_values[29], vy       );
		Utils::TryParse<double       >(_values[30], vz       );
		Utils::TryParse<double       >(_values[31], spect    );
		Utils::TryParse<std::string  >(_values[32], spect_src);
	}
	
	ATHYG::V3::V3(const std::array<std::string, 34>& _values) {

		Utils::TryParse<size_t       >(_values[ 0], id       );
		Utils::TryParse<std::string  >(_values[ 1], tyc      );
		Utils::TryParse<size_t       >(_values[ 2], gaia     );
		Utils::TryParse<size_t       >(_values[ 3], hyg      );
		Utils::TryParse<size_t       >(_values[ 4], hip      );
		Utils::TryParse<size_t       >(_values[ 5], hd       );
		Utils::TryParse<size_t       >(_values[ 6], hr       );
		Utils::TryParse<std::string  >(_values[ 7], gl       );
		Utils::TryParse<std::string  >(_values[ 8], bayer    );
		Utils::TryParse<std::string  >(_values[ 9], flam     );
		Utils::TryParse<std::string  >(_values[10], con      );
		Utils::TryParse<std::string  >(_values[11], proper   );
		Utils::TryParse<double       >(_values[12], ra       );
		Utils::TryParse<double       >(_values[13], dec      );
		Utils::TryParse<std::string  >(_values[14], pos_src  );
		Utils::TryParse<double       >(_values[15], dist     );
		Utils::TryParse<double       >(_values[16], x0       );
		Utils::TryParse<double       >(_values[17], y0       );
		Utils::TryParse<double       >(_values[18], z0       );
		Utils::TryParse<std::string  >(_values[19], dist_src );
		Utils::TryParse<double       >(_values[20], mag      );
		Utils::TryParse<double       >(_values[21], absmag   );
		Utils::TryParse<double       >(_values[22], ci       );
		Utils::TryParse<std::string  >(_values[23], mag_src  );
		Utils::TryParse<double       >(_values[24], rv       );
		Utils::TryParse<std::string  >(_values[25], rv_src   );
		Utils::TryParse<double       >(_values[26], pm_ra    );
		Utils::TryParse<double       >(_values[27], pm_dec   );
		Utils::TryParse<double       >(_values[28], pm_src   );
		Utils::TryParse<double       >(_values[29], vx       );
		Utils::TryParse<double       >(_values[30], vy       );
		Utils::TryParse<double       >(_values[31], vz       );
		Utils::TryParse<double       >(_values[32], spect    );
		Utils::TryParse<std::string  >(_values[33], spect_src);
    }
	
} // LouiEriksson::Engine::Spatial