set(Boost_INCLUDE_DIR /opt/qube/include/boost_1_61_0)
find_package(Boost REQUIRED)


if(Boost_FOUND)
	message(WARNING "Boost is found in ${Boost_INCLUDE_DIRS}" )
endif()


