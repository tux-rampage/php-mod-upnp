/*
 * upnp.c
 *
 *  Created on: 21.12.2012
 *      Author: unreality
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_upnp.h"

static function_entry upnp_functions[] = {
    PHP_FE(upnp_raw_lookup, NULL)
    {NULL, NULL, NULL}
};

extern "C" {
	zend_module_entry upnp_module_entry = {
	#if ZEND_MODULE_API_NO >= 20010901
		STANDARD_MODULE_HEADER,
	#endif
		PHP_UPNP_EXTNAME,
		upnp_functions,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	#if ZEND_MODULE_API_NO >= 20010901
		PHP_UPNP_VERSION,
	#endif
		STANDARD_MODULE_PROPERTIES
	};
};

#ifdef COMPILE_DL_UPNP
ZEND_GET_MODULE(upnp)
#endif

#include "UPnP.h"

PHP_FUNCTION(upnp_raw_lookup)
{
	char* serviccetype;
	int stsize = 0;
	long timeout = 5;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sl",
			&serviccetype,
			&stsize,
			&timeout) == FAILURE) {
		RETURN_NULL();
	}

	if (serviccetype == NULL) {
		serviccetype = "upnp:rootdevice";
	}

	try {
		modupnp::UPnP upnpinst;
		modupnp::UPnP::discoverres_t result = upnpinst.discover(serviccetype, timeout);

		zval* resultArray = NULL;
		ALLOC_INIT_ZVAL(resultArray);
		array_init(resultArray);

		modupnp::UPnP::discoverres_t::iterator it;
		for (it = result.begin(); it != result.end(); it++) {
			zval* headers = NULL;
			ALLOC_INIT_ZVAL(headers);
			array_init(headers);

			modupnp::UPnP::headerlist_t::iterator headerIter;
			for (headerIter = it->second.begin(); headerIter != it->second.end(); headerIter++) {
				add_next_index_string(headers, headerIter->c_str(), 1);
			}

			add_assoc_zval(resultArray, it->first.c_str(), headers);
		}

		RETURN_ZVAL(resultArray, 0, NULL);
	} catch (modupnp::UPnPException &e) {
		RETURN_FALSE;
	}
}



