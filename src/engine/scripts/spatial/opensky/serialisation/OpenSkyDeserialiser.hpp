#ifndef FINALYEARPROJECT_OPENSKYDESERIALISER_HPP
#define FINALYEARPROJECT_OPENSKYDESERIALISER_HPP

#include "../../../core/Types.hpp"
#include "../../../core/utils/Utils.hpp"

#include <json.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace LouiEriksson::Engine::Spatial::Serialisation {
	
	struct OpenSkyDeserialiser final {
	
		struct OpenSkyJSON final {
			
			struct Root final {
				
				/**
				 * @struct StateVector
				 * @brief Represents the state vector of an aircraft.
				 *
				 * @see <a href=https://openskynetwork.github.io/opensky-api/rest.html>OpenSky REST API Documentation</a>
				 */
				struct StateVector final {
					
					std::optional<        std::string> icao24;          /**< @brief Unique ICAO 24-bit address of the transponder in hex string representation.                                                  */
					std::optional<        std::string> callsign;        /**< @brief Callsign of the vehicle (8 chars). Can be null if no callsign has been received.                                             */
					std::optional<        std::string> origin_country;  /**< @brief Country name inferred from the ICAO 24-bit address.                                                                          */
					std::optional<             size_t> time_position;   /**< @brief Unix timestamp (seconds) for the last position update.                                                                       */
					std::optional<             size_t> last_contact;    /**< @brief Unix timestamp (seconds) for the last update in general.                                                                     */
					std::optional<           scalar_t> longitude;       /**< @brief WGS-84 longitude in decimal degrees.                                                                                         */
					std::optional<           scalar_t> latitude;        /**< @brief WGS-84 latitude in decimal degrees.                                                                                          */
					std::optional<           scalar_t> baro_altitude;   /**< @brief Barometric altitude in meters.                                                                                               */
					std::optional<               bool> on_ground;       /**< @brief Boolean value which indicates if the position was retrieved from a surface position report.                                  */
					std::optional<           scalar_t> velocity;        /**< @brief Velocity over ground in m/s.                                                                                                 */
					std::optional<           scalar_t> true_track;      /**< @brief True track in decimal degrees clockwise from north (north=0°).                                                               */
					std::optional<           scalar_t> vertical_rate;   /**< @brief Vertical rate in m/s. A positive value indicates that the airplane is climbing, a negative value indicates that it descends. */
					std::optional<std::vector<size_t>> sensors;         /**< @brief IDs of the receivers which contributed to this state vector.                                                                 */
					std::optional<           scalar_t> geo_altitude;    /**< @brief Geometric altitude in meters.                                                                                                */
					std::optional<        std::string> squawk;          /**< @brief The transponder code aka Squawk. Can be null.                                                                                */
					std::optional<               bool> spi;             /**< @brief Whether flight status indicates special purpose indicator.                                                                   */
					std::optional<             size_t> position_source; /**< @brief Origin of this state’s position.                                                                                             */
					std::optional<             size_t> category;        /**< @brief Aircraft category.                                                                                                           */
					
					StateVector(json&& _os_json) :
						icao24(), callsign(), origin_country(), time_position(), last_contact(),
						longitude(), latitude(), baro_altitude(), on_ground(), velocity(), true_track(),
						vertical_rate(), sensors(), geo_altitude(), squawk(), spi(), position_source(), category()
                    {
						try {
							
							try {          icao24 = _os_json[ 0U]; } catch (const json::exception& e) {         icao24 = std::nullopt; }
							try {        callsign = _os_json[ 1U]; } catch (const json::exception& e) {       callsign = std::nullopt; }
							try {  origin_country = _os_json[ 2U]; } catch (const json::exception& e) { origin_country = std::nullopt; }
							try {   time_position = _os_json[ 3U]; } catch (const json::exception& e) {  time_position = std::nullopt; }
							try {    last_contact = _os_json[ 4U]; } catch (const json::exception& e) {   last_contact = std::nullopt; }
							try {       longitude = _os_json[ 5U]; } catch (const json::exception& e) {      longitude = std::nullopt; }
							try {        latitude = _os_json[ 6U]; } catch (const json::exception& e) {       latitude = std::nullopt; }
							try {   baro_altitude = _os_json[ 7U]; } catch (const json::exception& e) {  baro_altitude = std::nullopt; }
							try {       on_ground = _os_json[ 8U]; } catch (const json::exception& e) {      on_ground = std::nullopt; }
							try {        velocity = _os_json[ 9U]; } catch (const json::exception& e) {       velocity = std::nullopt; }
							try {      true_track = _os_json[10U]; } catch (const json::exception& e) {     true_track = std::nullopt; }
							try {   vertical_rate = _os_json[11U]; } catch (const json::exception& e) {  vertical_rate = std::nullopt; }
							
							try {
								
								auto jSensors = _os_json[12U];
					
								auto s = std::vector<size_t>();
								s.reserve(jSensors.size());
								for (const auto& item : jSensors) {
									s.emplace_back(item);
								}
								
								sensors = s;
							}
							catch (const json::exception& e) {
								sensors = std::nullopt;
							}
							
							try {    geo_altitude = _os_json[13U]; } catch (const json::exception& e) {    geo_altitude = std::nullopt; }
							try {          squawk = _os_json[14U]; } catch (const json::exception& e) {          squawk = std::nullopt; }
							try {             spi = _os_json[15U]; } catch (const json::exception& e) {             spi = std::nullopt; }
							try { position_source = _os_json[16U]; } catch (const json::exception& e) { position_source = std::nullopt; }
							try {        category = _os_json[17U]; } catch (const json::exception& e) {        category = std::nullopt; }
							
						}
						catch (const std::exception& e) {
							Debug::Log(e);
						}
					}
				};
				
				/**
				 * @brief The time in seconds since epoch (Unix time stamp to retrieve states for.
				 *
				 * @note Current time will be used if omitted.
				 */
				const size_t time;
				
				/**
				 * @brief One or more ICAO24 transponder addresses represented by a hex string (e.g. abc9f3).
				 *
				 * @note To filter multiple ICAO24 append the property once for each address.
				 *       If omitted, the state vectors of all aircraft are returned.
				 */
				std::vector<StateVector> states;
				
				Root(json&& _os_json) :
					time(_os_json["time"]),
					states()
				{
					auto jStates = _os_json["states"];
			
					states.reserve(jStates.size());
					for (auto& item : jStates) {
						states.emplace_back(std::move(item));
					}
				}
			};

			const Root m_Root;
			
			OpenSkyJSON(json&& _os_json) : m_Root(std::move(_os_json)) {}
		};
		
		template <typename T>
		static T Deserialise(std::istringstream&& _data) {
			return { json::parse(std::move(_data)) };
		}
	};
}


#endif //FINALYEARPROJECT_OPENSKYDESERIALISER_HPP