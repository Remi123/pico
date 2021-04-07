set(Boost_INCLUDE_DIR /usr/local/boost_1_75_0)
find_package(Boost REQUIRED)


if(Boost_FOUND)
	message(WARNING "Boost is found in ${Boost_INCLUDE_DIRS}" )
endif()


