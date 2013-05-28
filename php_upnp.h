/*
 * php_upnp.h
 *
 *  Created on: 21.12.2012
 *      Author: unreality
 */

#ifndef PHP_UPNP_H_
#define PHP_UPNP_H_

#include <php.h>
#define PHP_UPNP_VERSION "1.0"
#define PHP_UPNP_EXTNAME "upnp"

PHP_FUNCTION(upnp_raw_lookup);

extern "C" {
    extern zend_module_entry upnp_module_entry;
}

#define phpext_upnp_ptr &upnp_module_entry

#endif /* PHP_UPNP_H_ */
