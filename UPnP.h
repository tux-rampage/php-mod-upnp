/*
 * UPnP.hpp
 *
 *  Created on: 21.12.2012
 *      Author: unreality
 */

#ifndef UPNP_HPP_
#define UPNP_HPP_

#include <map>
#include <vector>
#include <string>

namespace modupnp
{
	/**
	 * Exception class for UPnP
	 */
	class UPnPException : std::exception
	{
	};

	/**
	 * UPnP class
	 */
	class UPnP
	{
		public:
			typedef std::vector<std::string> headerlist_t;
		    typedef std::map<std::string, headerlist_t> discoverres_t;

			static const char* UPNP_BCAST_ADDR;
			static const int   UPNP_BCAST_PORT = 1900;

		public:
			inline UPnP() {};

			/**
			 * Discover UPnP devices and return raw headers
			 */
		    discoverres_t discover(const char* servicetype, long timeout);
	};

	//extern static const char* UPnP::UPNP_BCAST_ADDR;
};

#endif /* UPNP_HPP_ */
