PHP_ARG_ENABLE(upnp, Enable simple upnp functions, [  --enable-upnp	Enable simple upnp functions])

if test "$PHP_UPNP" = "yes"; then
    AC_DEFINE(HAVE_UPNP, 1, [Whether you have UPnP])
    PHP_REQUIRE_CXX()
    PHP_SUBST(UPNP_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, UPNP_SHARED_LIBADD)

  	PHP_NEW_EXTENSION(upnp, mod_upnp.cc UPnP.cc, $ext_shared)
fi