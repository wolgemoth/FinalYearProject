#ifndef FINALYEARPROJECT_CONVERSIONS_H
#define FINALYEARPROJECT_CONVERSIONS_H

#include "../../core/utils/Hashmap.h"

#include <string>

namespace LouiEriksson::Engine::Spatial::Maths {

	struct Conversions {
		
		struct Speed {
		
		public:
			
			enum Unit : char {
				KilometreHour,
				FeetSecond,
				MileHour,
				Knot,
				MetreSecond,
				Mach,
				Lightspeed,
			};
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
			
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);
			
			static std::string Symbol(const Unit& _unit);
			
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
				{ "k/h",   KilometreHour },
				{ "km/h",  KilometreHour },
				{ "kph",   KilometreHour },
				{ "f/s",   FeetSecond    },
				{ "fps",   FeetSecond    },
				{ "mi/h",  MileHour      },
				{ "mph",   MileHour      },
				{ "kn",    Knot          },
				{ "kt",    Knot          },
				{ "knot",  Knot          },
				{ "knots", Knot          },
				{ "nmi/h", Knot          },
				{ "nmiph", Knot          },
	            { "m/s",   MetreSecond   },
	            { "mps",   MetreSecond   },
				{ "mach",  Mach          },
				{ "c",     Lightspeed    },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ KilometreHour, "km/h" },
				{ FeetSecond,    "f/s"  },
				{ MileHour,      "mph"  },
				{ Knot,          "kn"   },
				{ MetreSecond,   "m/s"  },
				{ Mach,          "mach" },
				{ Lightspeed,    "c"    },
			};
			
			/// <summary>
			/// Conversions between common speed units and m/s.
			/// </summary>
			inline static Hashmap<Unit, float> s_Conversion = {
				{ KilometreHour, 0.2777778f   },
				{ FeetSecond,    0.3048f      },
				{ MileHour,      0.44704f     },
				{ Knot,          0.514444f    },
	            { MetreSecond,   1.0f         },
				{ Mach,          340.29f      },
				{ Lightspeed,    299792458.0f },
			};
		};
		
		struct Distance {
		
		public:
			
			enum Unit : char {
				Millimetre,
				Centimetre,
				Inch,
				Foot,
				Yard,
				Metre,
				Kilometre,
				Mile,
				NauticalMile,
				AstronomicalUnit,
				Lightyear,
				Parsec,
			};
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
		
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);

			static std::string Symbol(const Unit& _unit);
		
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
	            { "mm",         Millimetre       },
	            { "cm",         Centimetre       },
	            { "\"",         Inch             },
	            { "in",         Inch             },
	            { "f",          Foot             },
	            { "\'",         Foot             },
	            { "ft",         Foot             },
	            { "yards",      Yard             },
	            { "yard",       Yard             },
	            { "yd",         Yard             },
	            { "m",          Metre            },
	            { "km",         Kilometre        },
	            { "mi",         Mile             },
	            { "nmi",        NauticalMile     },
				{ "au",         AstronomicalUnit },
	            { "ly",         Lightyear        },
	            { "lightyear",  Lightyear        },
	            { "lightyears", Lightyear        },
	            { "pc",         Parsec           },
	            { "parsec",     Parsec           },
	            { "parsecs",    Parsec           },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ Millimetre,       "mm"  },
				{ Centimetre,       "cm"  },
				{ Inch,             "in"  },
				{ Foot,             "ft"  },
				{ Yard,             "yd"  },
				{ Metre,             "m"  },
				{ Kilometre,        "km"  },
				{ Mile,             "mi"  },
				{ NauticalMile,     "nmi" },
				{ AstronomicalUnit, "au"  },
				{ Lightyear,        "ly"  },
				{ Parsec,           "pc"  },
			};
			
			/// <summary>
			/// Conversions between common lateral distance units and metres.
			/// </summary>
			inline static Hashmap<Unit, float> s_Conversion = {
	            { Millimetre,                       0.001f      },
	            { Centimetre,                       0.01f       },
	            { Inch,                             0.0254f     },
	            { Foot,                             0.30479999f },
	            { Yard,                             0.9144f     },
	            { Metre,                            1.0f        },
	            { Kilometre,                     1000.0f        },
	            { Mile,                          1609.344f      },
	            { NauticalMile,                  1852.0f        },
				{ AstronomicalUnit,      149597870700.0f        },
	            { Lightyear,         9460730472580800.0f        },
	            { Parsec,           30856775810000000.0f        },
			};
			
		};
			
		struct Rotation {
		
		public:
			
			enum Unit : char {
				Gradian,
				Degree,
				Radian,
				Turn,
			};
			
			inline static constexpr float s_DegreesToRadians = M_PI / 180.0f;
			
			inline static constexpr float s_RadiansToDegrees = 180.0f / M_PI;
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
			
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);

			static std::string Symbol(const Unit& _unit);
		
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
				{ "grad",     Gradian },
				{ "gradians", Gradian },
	            { "°",        Degree  },
	            { "d",        Degree  },
	            { "deg",      Degree  },
	            { "degree",   Degree  },
	            { "degrees",  Degree  },
				{ "rad",      Radian  },
				{ "radians",  Radian  },
				{ "turns",    Turn    },
				{ "turn",     Turn    },
				{ "cycle",    Turn    },
				{ "pla",      Turn    },
				{ "rev",      Turn    },
				{ "tr",       Turn    },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ Gradian, "grad" },
				{ Degree,  "deg"  },
				{ Radian,  "rad"  },
				{ Turn,    "tr"   },
			};
			
			/// <summary>
			/// Conversions between common rotational units and degrees.
			/// </summary>
			inline static Hashmap<Unit, float> s_Conversion = {
				{ Gradian,  0.9f     },
	            { Degree,   1.0f     },
	            { Radian,  57.29578f },
	            { Turn,   360.0f     },
			};
		};
		
		struct Time {
		
		public:
			
			enum Unit : char {
				Nanosecond,
				Microsecond,
				Millisecond,
				Second,
				Minute,
				Hour,
				Day,
			};
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
			
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);

			static std::string Symbol(const Unit& _unit);
			
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
			    { "nanosecond",   Nanosecond  },
	            { "nanoseconds",  Nanosecond  },
	            { "ns",           Nanosecond  },
				{ "microsecond",  Microsecond },
				{ "microseconds", Microsecond },
				{ "µs",           Microsecond },
	            { "millisecond",  Millisecond },
	            { "milliseconds", Millisecond },
	            { "ms",           Millisecond },
	            { "s",            Second      },
	            { "sec",          Second      },
	            { "seconds",      Second      },
	            { "secs",         Second      },
				{ "m",            Minute      },
				{ "min",          Minute      },
				{ "minute",       Minute      },
				{ "minutes",      Minute      },
				{ "h",            Hour        },
				{ "hour",         Hour        },
				{ "hours",        Hour        },
				{ "hr",           Hour        },
				{ "d",            Day         },
				{ "day",          Day         },
				{ "days",         Day         },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ Nanosecond,  "ns" },
				{ Microsecond, "µs" },
				{ Millisecond, "ms" },
				{ Second,       "s" },
				{ Minute,       "m" },
				{ Hour,         "h" },
				{ Day,          "d" },
			};
			
			/// <summary>
			/// Conversions between common time units and seconds.
			/// </summary>
			inline static Hashmap<Unit, float> s_Conversion = {
	            { Nanosecond,      0.000000001f },
				{ Microsecond,     0.000001f    },
	            { Millisecond,     0.001f       },
	            { Second,          1.0f         },
				{ Minute,         60.0f         },
				{ Hour,         3600.0f         },
				{ Day,         86400.0f         },
			};
		};
		
		struct Temperature {
		
		public:
			
			enum Unit : char {
				Celsius,
				Fahrenheit,
				Kelvin,
			};
			
			static constexpr float s_PlanckTemperature = 14200000000000000000000000000000000.0f;
			static constexpr float s_AbsoluteZero      =                                   0.0f;
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
			
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);

			static std::string Symbol(const Unit&);
			
			static float ClampTemperature(const float& _val, Unit& _unit);
			
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
	            { "celsius",     Celsius    },
	            { "c",           Celsius    },
	            { "°c",          Celsius    },
	            { "°C",          Celsius    },
				{ "fahrenheit",  Fahrenheit },
	            { "f",           Fahrenheit },
	            { "°f",          Fahrenheit },
	            { "°F",          Fahrenheit },
				{ "kelvin",      Kelvin     },
	            { "k",           Kelvin     },
	            { "K",           Kelvin     },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ Celsius,    "C" },
				{ Fahrenheit, "F" },
				{ Kelvin,     "K" },
			};
			
		};
		
		struct Pressure {
		
		public:
			
			enum Unit : char {
				DyneSquareCentimetre,
				MilliTorr,
				Pascal,
				MillimetreWater,
				PoundSquareFoot,
				Hectopascal,
				CentimetreWater,
				MillimetreMercury,
				InchWater,
				OunceSquareInch,
				Decibel,
				Kilopascal,
				CentimetreMercury,
				FeetWater,
				InchMercury,
				PoundSquareInch,
				MetreWater,
				TonneSquareFoot_Short,
				TechnicalAtmosphere,
				KilogramSquareCentimetre,
				Bar,
				Atmosphere,
				Megapascal,
				TonneSquareInch_Short,
				TonneSquareInch_Long,
			};
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
			
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);

			static std::string Symbol(const Unit& _unit);
			
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
				{ "dyn/cm²",      DyneSquareCentimetre          },
				{ "dyn/cm^2",     DyneSquareCentimetre          },
				{ "dyn/cm2",      DyneSquareCentimetre          },
				{ "mTorr",        MilliTorr                     },
	            { "pascals",      Pascal                        },
	            { "pascal",       Pascal                        },
	            { "pa",           Pascal                        },
	            { "Pa",           Pascal                        },
				{ "N/m²",         Pascal                        },
				{ "N/m^2",        Pascal                        },
				{ "N/m2",         Pascal                        },
				{ "mmH2O",        MillimetreWater               },
				{ "psf",          PoundSquareFoot               },
	            { "millibars",    Hectopascal                   },
	            { "millibar",     Hectopascal                   },
	            { "mbar",         Hectopascal                   },
				{ "hPa",          Hectopascal                   },
				{ "hectopascals", Hectopascal                   },
				{ "hectopascal",  Hectopascal                   },
				{ "cmH2O",        CentimetreWater               },
				{ "mmHg",         MillimetreMercury             },
				{ "inH20",        InchWater                     },
				{ "oz/in²",       OunceSquareInch               },
				{ "oz/in^2",      OunceSquareInch               },
				{ "oz/in2",       OunceSquareInch               },
				{ "dB",           Decibel                       },
				{ "decibel",      Decibel                       },
				{ "decibels",     Decibel                       },
				{ "kpa",          Kilopascal                    },
				{ "kPa",          Kilopascal                    },
				{ "kilopascals",  Kilopascal                    },
				{ "kilopascal",   Kilopascal                    },
				{ "cmHg",         CentimetreMercury             },
				{ "ftH2O",        FeetWater                     },
				{ "inHg",         InchMercury                   },
				{ "psi",          PoundSquareInch               },
				{ "mH2O",         MetreWater                    },
				{ "tsf",          TonneSquareFoot_Short         },
				{ "tsf_us",       TonneSquareFoot_Short         },
				{ "tsf_short",    TonneSquareFoot_Short         },
	            { "at",           TechnicalAtmosphere           },
				{ "kg/cm²",       KilogramSquareCentimetre      },
				{ "kg/cm^2",      KilogramSquareCentimetre      },
				{ "kg/cm2",       KilogramSquareCentimetre      },
	            { "bars",         Bar                           },
	            { "bar",          Bar                           },
	            { "atmospheres",  Atmosphere                    },
	            { "atmosphere",   Atmosphere                    },
	            { "atm",          Atmosphere                    },
				{ "MPa",          Megapascal                    },
				{ "megapascals",  Megapascal                    },
				{ "megapascal",   Megapascal                    },
				{ "tsi",          TonneSquareInch_Short         },
				{ "tsi_us",       TonneSquareInch_Short         },
				{ "tsi_short",    TonneSquareInch_Short         },
				{ "tsi_uk",       TonneSquareInch_Long          },
				{ "tsi_long",     TonneSquareInch_Long          },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ DyneSquareCentimetre,    "dyn/cm2",   },
				{ MilliTorr,               "mTorr",     },
				{ Pascal,                  "Pa",        },
				{ MillimetreWater,         "mmH2O",     },
				{ PoundSquareFoot,         "psf",       },
				{ Hectopascal,             "hPa",       },
				{ CentimetreWater,         "cmH2O",     },
				{ MillimetreMercury,       "mmHg",      },
				{ InchWater,               "inH2O",     },
				{ OunceSquareInch,         "oz/in2",    },
				{ Decibel,                 "dB",        },
				{ Kilopascal,              "kPa",       },
				{ CentimetreMercury,       "cmHg",      },
				{ FeetWater,               "ftH2O",     },
				{ InchMercury,             "inHg",      },
				{ PoundSquareInch,         "psi",       },
				{ MetreWater,              "mH2O",      },
				{ TonneSquareFoot_Short,   "tsf_short", },
				{ TechnicalAtmosphere,     "at",        },
				{ KilogramSquareCentimetre,"kg/cm2",    },
				{ Bar,                     "bar",       },
				{ Atmosphere,              "atm",       },
				{ Megapascal,              "MPa",       },
				{ TonneSquareInch_Short,   "tsi_short", },
				{ TonneSquareInch_Long,    "tsi_long",  },
			};

			/// <summary>
			/// Conversions between common pressure units and "atmospheres".
			/// Source: https://www.sensorsone.com/atm-standard-atmosphere-pressure-unit/
			/// </summary>
			inline static Hashmap<Unit, float> s_Conversion = {
				{ DyneSquareCentimetre,       0.000000987f },
				{ MilliTorr,                  0.000001316f },
				{ Pascal,                     0.000009869f },
				{ MillimetreWater,            0.000096784f },
				{ PoundSquareFoot,            0.000472541f },
				{ Hectopascal,                0.000986923f },
				{ CentimetreWater,            0.000967839f },
				{ MillimetreMercury,          0.001315789f },
				{ InchWater,                  0.002458319f },
				{ OunceSquareInch,            0.004252876f },
				{ Decibel,                    0.005154639f },
				{ Kilopascal,                 0.009869233f },
				{ CentimetreMercury,          0.013157895f },
				{ FeetWater,                  0.02949983f  },
				{ InchMercury,                0.033421008f },
				{ PoundSquareInch,            0.06804619f  },
				{ MetreWater,                 0.096783872f },
				{ TonneSquareFoot_Short,      0.945081324f },
	            { TechnicalAtmosphere,        0.967838719f },
				{ KilogramSquareCentimetre,   0.967838719f },
	            { Bar,                        0.986923267f },
	            { Atmosphere,                 1.0f         },
				{ Megapascal,                 9.869232667f },
				{ TonneSquareInch_Short,    136.092009086f },
				{ TonneSquareInch_Long,     152.422992094f },
			};
			
		};
		
		struct Mass {
		
		public:
			
			enum Unit : char {
				Nanogram,
				Microgram,
				Milligram,
				Gram,
				Ounce,
				Pound,
				Kilogram,
				Ton,
				Kiloton,
				Megaton,
				Gigaton,
			};
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
			
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);

			static std::string Symbol(const Unit& _unit);
			
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
					{ "nanogram",     Nanogram  },
					{ "nanogramme",   Nanogram  },
					{ "nanogrammes",  Nanogram  },
					{ "nanograms",    Nanogram  },
					{ "ng",           Nanogram  },
					{ "microgram",    Microgram },
					{ "microgramme",  Microgram },
					{ "microgrammes", Microgram },
					{ "micrograms",   Microgram },
					{ "μg",           Microgram },
					{ "mg",           Milligram },
					{ "milligram",    Milligram },
					{ "milligramme",  Milligram },
					{ "milligrammes", Milligram },
					{ "milligrams",   Milligram },
					{ "g",            Gram      },
					{ "gram",         Gram      },
					{ "gramme",       Gram      },
					{ "grammes",      Gram      },
					{ "grams",        Gram      },
					{ "ounce",        Ounce     },
					{ "oz",           Ounce     },
					{ "lb",           Pound     },
					{ "pound",        Pound     },
					{ "kg",           Kilogram  },
					{ "kilogram",     Kilogram  },
					{ "kilogramme",   Kilogram  },
					{ "kilogrammes",  Kilogram  },
					{ "kilograms",    Kilogram  },
					{ "t",            Ton       },
					{ "ton",          Ton       },
					{ "tonne",        Ton       },
					{ "tonnes",       Ton       },
					{ "tons",         Ton       },
					{ "kilotonne",    Kiloton   },
					{ "kiloton",      Kiloton   },
					{ "kilotonnes",   Kiloton   },
					{ "kilotons",     Kiloton   },
					{ "kt",           Kiloton   },
					{ "megaton",      Megaton   },
					{ "megatonne",    Megaton   },
					{ "megatonnes",   Megaton   },
					{ "megatons",     Megaton   },
					{ "Mt",           Megaton   },
					{ "gigaton",      Gigaton   },
					{ "gigatonne",    Gigaton   },
					{ "gigatonnes",   Gigaton   },
					{ "gigatons",     Gigaton   },
					{ "Gt",           Gigaton   },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ Nanogram,  "ng" },
				{ Microgram, "μg" },
				{ Milligram, "mg" },
				{ Gram,      "g"  },
				{ Ounce,     "oz" },
				{ Pound,     "lb" },
				{ Kilogram,  "kg" },
				{ Ton,       "t"  },
				{ Kiloton,   "kt" },
				{ Megaton,   "Mt" },
				{ Gigaton,   "Gt" },
			};
			
			/// <summary>
			/// Conversions between common mass units and kilograms.
			/// </summary>
			inline static Hashmap<Unit, float> s_Conversion = {
					{ Nanogram,              0.000000000001f },
					{ Microgram,             0.000000001f    },
					{ Milligram,             0.000001f       },
					{ Gram,                  0.001f          },
					{ Ounce,                 0.02834952f     },
					{ Pound,                 0.4535923f      },
					{ Kilogram,              1.0f            },
					{ Ton,                1000.0f            },
					{ Kiloton,         1000000.0f            },
					{ Megaton,      1000000000.0f            },
					{ Gigaton,   1000000000000.0f            },
			};
			
		};
		
		struct Area {
		
		public:
			
			enum Unit : char {
				SquareMillimetre,
				SquareCentimetre,
				SquareInch,
				SquareMetre,
				SquareFoot,
				Acre,
				Hectare,
				SquareYard,
			};
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
			
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);

			static std::string Symbol(const Unit& _unit);
			
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
				{ "mm2",     SquareMillimetre },
				{ "mm^2",    SquareMillimetre },
				{ "mm²",     SquareMillimetre },
				{ "cm2",     SquareCentimetre },
				{ "cm^2",    SquareCentimetre },
				{ "cm²",     SquareCentimetre },
				{ "\"²",     SquareInch       },
				{ "in2",     SquareInch       },
				{ "in^2",    SquareInch       },
				{ "in²",     SquareInch       },
				{ "\'2",     SquareFoot       },
				{ "ft2",     SquareFoot       },
				{ "ft^2",    SquareFoot       },
				{ "ft²",     SquareFoot       },
				{ "yd2",     SquareYard       },
				{ "yd^2",    SquareYard       },
				{ "yd²",     SquareYard       },
				{ "m2",      SquareMetre      },
				{ "m^2",     SquareMetre      },
				{ "m²",      SquareMetre      },
				{ "ac",      Acre             },
				{ "acre",    Acre             },
				{ "ha",      Hectare          },
				{ "hectare", Hectare          },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ SquareMillimetre, "mm2" },
				{ SquareCentimetre, "cm2" },
				{ SquareInch,       "in2" },
				{ SquareMetre,      "m2"  },
				{ SquareFoot,       "ft2" },
				{ Acre,             "ac"  },
				{ Hectare,          "ha"  },
				{ SquareYard,       "yd2" },
			};
			
			/// <summary>
			/// Conversions between common mass units and square metres.
			/// </summary>
			inline static Hashmap<Unit, float> s_Conversion = {
				{ SquareMillimetre,    0.000001f     },
				{ SquareCentimetre,    0.0001f       },
				{ SquareInch,          0.00064516f   },
				{ SquareFoot,          0.09290304f   },
				{ SquareYard,          0.83612736f   },
				{ SquareMetre,         1.0f          },
				{ Acre,             4046.8564224f    },
				{ Hectare,         10000.0f          },
			};
		
		};
		
		struct Volume {
		
		public:
			
			enum Unit : char {
				Millilitre,
				Centilitre,
				CubicInch,
				FluidOunce,
				Cup,
				Pint,
				Quart,
				Litre,
				Gallon,
				CubicFoot,
				Barrel,
				CubicYard,
				CubicMetre,
			};
			
			static bool TryGuessUnit(const std::string& _symbol, Unit& _result);
			
			[[nodiscard]] static float Convert(const float& _val, const Unit& _from, const Unit& _to);

			static std::string Symbol(const Unit& _unit);
			
		private:
			
			inline static Hashmap<std::string, Unit> s_Lookup = {
				{ "milliliter", Millilitre },
				{ "millilitre", Millilitre },
				{ "ml",         Millilitre },
				{ "centiliter", Centilitre },
				{ "centilitre", Centilitre },
				{ "cl",         Centilitre },
				{ "\"3",        CubicInch  },
				{ "\"^3",       CubicInch  },
				{ "\"³",        CubicInch  },
				{ "cu in",      CubicInch  },
				{ "cu. in",     CubicInch  },
				{ "cu. in.",    CubicInch  },
				{ "in. cu",     CubicInch  },
				{ "in. cu.",    CubicInch  },
				{ "in3",        CubicInch  },
				{ "in^3",       CubicInch  },
				{ "in³",        CubicInch  },
				{ "fl oz",      FluidOunce },
				{ "fl ℥",       FluidOunce },
				{ "fl. oz",     FluidOunce },
				{ "fl/oz",      FluidOunce },
				{ "floz",       FluidOunce },
				{ "f℥",         FluidOunce },
				{ "oz. fl",     FluidOunce },
				{ "oz. fl.",    FluidOunce },
				{ "ƒ ℥",        FluidOunce },
				{ "℥",          FluidOunce },
				{ "cup",        Cup        },
				{ "cups",       Cup        },
				{ "p",          Pint       },
				{ "pint",       Pint       },
				{ "pt",         Pint       },
				{ "qt",         Quart      },
				{ "quart",      Quart      },
				{ "l",          Litre      },
				{ "liter",      Litre      },
				{ "litre",      Litre      },
				{ "gal",        Gallon     },
				{ "gallon",     Gallon     },
				{ "\'3",        CubicFoot  },
				{ "\'^3",       CubicFoot  },
				{ "\'³",        CubicFoot  },
				{ "cu f",       CubicFoot  },
				{ "cu ft",      CubicFoot  },
				{ "cu. f",      CubicFoot  },
				{ "cu. f.",     CubicFoot  },
				{ "cu. ft",     CubicFoot  },
				{ "cu. ft.",    CubicFoot  },
				{ "f. cu",      CubicFoot  },
				{ "f. cu.",     CubicFoot  },
				{ "f^3",        CubicFoot  },
				{ "ft. cu",     CubicFoot  },
				{ "ft. cu.",    CubicFoot  },
				{ "ft3",        CubicFoot  },
				{ "ft^3",       CubicFoot  },
				{ "ft³",        CubicFoot  },
				{ "f³",         CubicFoot  },
				{ "in3",        CubicFoot  },
				{ "barrel",     Barrel     },
				{ "barrels",    Barrel     },
				{ "bbl",        Barrel     },
				{ "yd3",        CubicYard  },
				{ "yd^3",       CubicYard  },
				{ "yd³",        CubicYard  },
				{ "m3",         CubicMetre },
				{ "m^3",        CubicMetre },
				{ "m³",         CubicMetre },
			};
			
			inline static Hashmap<Unit, std::string> s_Symbol = {
				{ Millilitre,      "ml"     },
				{ Centilitre,      "cl"     },
				{ CubicInch,       "in3"    },
				{ FluidOunce,      "fl. oz" },
				{ Cup,             "cup"    },
				{ Pint,            "pt"     },
				{ Quart,           "qt"     },
				{ Litre,           "l"      },
				{ Gallon,          "gal"    },
				{ CubicFoot,       "ft3"    },
				{ Barrel,          "bbl"    },
				{ CubicYard,       "yd3"    },
				{ CubicMetre,      "m3"     },
			};
			
			/// <summary>
			/// Conversions between common mass units and cubic metres.
			/// </summary>
			inline static Hashmap<Unit, float> s_Conversion = {
				{ Millilitre, 0.000001f       },
				{ Centilitre, 0.00001f        },
				{ CubicInch,  0.000016387064f },
				{ FluidOunce, 0.000029574f    },
				{ Cup,        0.000237f       },
				{ Pint,       0.000473176473f },
				{ Quart,      0.000946f       },
				{ Litre,      0.001f          },
				{ Gallon,     0.003785411784f },
				{ CubicFoot,  0.028316846592f },
				{ Barrel,     0.158987294928f },
				{ CubicYard,  0.764554858f    },
				{ CubicMetre, 1.0f            },
			};
		};
	};
	
} // LouiEriksson::Engine::Spatial::Maths

#endif //FINALYEARPROJECT_CONVERSIONS_H