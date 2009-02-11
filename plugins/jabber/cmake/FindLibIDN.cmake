# - Try to find GNUTLS 
# Find GNUTLS headers, libraries and the answer to all questions.
#
#  GNUTLS_FOUND               True if gnutls got found
#  GNUTLS_INCLUDE_DIRS        Location of gnutls headers 
#  GNUTLS_LIBRARIES           List of libaries to use gnutls 
#
# Copyright (c) 2007 Bjoern Ricks <b.ricks@fh-osnabrueck.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

FIND_PATH( LIBIDN_INCLUDE_DIR stringprep.h )
FIND_LIBRARY( LIBIDN_LIBRARIES idn )

if( LIBIDN_LIBRARIES AND LIBIDN_INCLUDE_DIR )
	message( STATUS "Found libidn: ${LIBIDN_LIBRARIES}" )
	ADD_DEFINITIONS( -DLIBIDN_FOUND )
else( LIBIDN_LIBRARIES AND LIBIDN_INCLUDE_DIR )
	message( SEND_ERROR "Could NOT find libidn" )
endif( LIBIDN_LIBRARIES AND LIBIDN_INCLUDE_DIR )
