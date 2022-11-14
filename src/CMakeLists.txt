PROJECT(Serverlib)
SET(PROJECT_SERVER_LIB ${PROJECT_NAME})

ADD_LIBRARY(${PROJECT_SERVER_LIB} SHARED
    server/Core.h
    server/Core.cpp
    server/RequestHandler.h
    server/RequestHandler.cpp
    server/Server.h
    server/Server.cpp
    server/Session.h
    server/Session.cpp
    server/user/Balance.h
    server/user/Balance.cpp
    server/user/Offer.h
    server/user/Offer.cpp
    server/user/User.h
    server/user/User.cpp

    hpp/Common.hpp
    hpp/json.hpp
    )
TARGET_LINK_LIBRARIES(${PROJECT_SERVER_LIB} PRIVATE Threads::Threads ${Boost_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_SERVER_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
SET(SERVER_LIB_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${PROJECT_SERVER_LIB}.so)

################################

PROJECT(Server)
SET(PROJECT_SERVER ${PROJECT_NAME})

ADD_EXECUTABLE(${PROJECT_SERVER}
    server/main.cpp
    )
TARGET_LINK_LIBRARIES(${PROJECT_SERVER} ${SERVER_LIB_PATH})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_SERVER} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

################################

PROJECT(Clientlib)
SET(PROJECT_CLIENT_LIB ${PROJECT_NAME})

ADD_LIBRARY(${PROJECT_CLIENT_LIB} SHARED
    client/Client.h
    client/Client.cpp

    hpp/Common.hpp
    hpp/json.hpp
    )
TARGET_LINK_LIBRARIES(${PROJECT_CLIENT_LIB} PRIVATE Threads::Threads ${Boost_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_CLIENT_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
SET(CLIENT_LIB_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${PROJECT_CLIENT_LIB}.so)

################################

PROJECT(Client)
SET(PROJECT_CLIENT ${PROJECT_NAME})

ADD_EXECUTABLE(${PROJECT_CLIENT}
    client/main.cpp
    )
TARGET_LINK_LIBRARIES(${PROJECT_CLIENT} ${CLIENT_LIB_PATH})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_CLIENT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

################################

PROJECT(Tests)
SET(PROJECT_TESTS ${PROJECT_NAME})

FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

ADD_EXECUTABLE(${PROJECT_TESTS}
    tests/main.cpp

    tests/TestAccountFunctionality/TestAuthorization.h
    tests/TestAccountFunctionality/TestAuthorization.cpp
    tests/TestAccountFunctionality/TestRegistration.h
    tests/TestAccountFunctionality/TestRegistration.cpp
    tests/TestAccountFunctionality/TestRemovingUser.h
    tests/TestAccountFunctionality/TestRemovingUser.cpp

    tests/TestUserFunctionality/TestBasic.h
    tests/TestUserFunctionality/TestBasic.cpp
    tests/TestUserFunctionality/TestOfferCreation.h
    tests/TestUserFunctionality/TestOfferCreation.cpp
    tests/TestUserFunctionality/TestOfferMerging.h
    tests/TestUserFunctionality/TestOfferMerging.cpp
    tests/TestUserFunctionality/TestShowAllOffers.h
    tests/TestUserFunctionality/TestShowAllOffers.cpp
    tests/TestUserFunctionality/TestShowStatistic.h
    tests/TestUserFunctionality/TestShowStatistic.cpp
    )
TARGET_LINK_LIBRARIES(${PROJECT_TESTS} ${GTEST_LIBRARIES} pthread)
TARGET_LINK_LIBRARIES(${PROJECT_TESTS} ${SERVER_LIB_PATH})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_TESTS} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

