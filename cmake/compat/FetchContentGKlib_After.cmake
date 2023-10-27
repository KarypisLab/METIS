set_property(GLOBAL APPEND PROPERTY PACKAGES_FOUND GKlib)
get_property(_packages_not_found GLOBAL PROPERTY PACKAGES_NOT_FOUND)
list(REMOVE_ITEM _packages_not_found GKlib)
set_property(GLOBAL PROPERTY PACKAGES_NOT_FOUND ${_packages_not_found})
