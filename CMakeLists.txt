cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "djd-tp")
project(${PROJECT_NAME})

find_package(SDL2 2.32.2 EXACT REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
)

link_directories(
        ${SDL2_LIBRARY_DIRS}
        ${SDL2_IMAGE_LIBRARY_DIRS}
        ${SDL2_TTF_LIBRARY_DIRS}
        ${SDL2_MIXER_LIBRARY_DIRS}
)

add_executable(${PROJECT_NAME}
        Source/Utils/Math.cpp
        Source/Utils/Random.cpp
        Source/Utils/Json.h
        Source/Utils/CSV.h
        Source/Main.cpp
        Source/Game.cpp
        Source/StatScreen.cpp
        Source/StatScreen.h
        Source/Actors/Actor.cpp
        Source/Components/Component.cpp
        Source/Components/RigidBodyComponent.cpp
        Source/Components/RigidBodyComponent.h
        Source/Components/ColliderComponents/CircleColliderComponent.cpp
        Source/Components/ColliderComponents/CircleColliderComponent.h
        Source/Components/ColliderComponents/AABBColliderComponent.cpp
        Source/Components/ColliderComponents/AABBColliderComponent.h
        Source/Components/DrawComponents/DrawComponent.cpp
        Source/Components/DrawComponents/DrawComponent.h
        Source/Components/DrawComponents/DrawSpriteComponent.cpp
        Source/Components/DrawComponents/DrawSpriteComponent.h
        Source/Components/DrawComponents/DrawPolygonComponent.cpp
        Source/Components/DrawComponents/DrawPolygonComponent.h
        Source/Components/DrawComponents/DrawAnimatedComponent.cpp
        Source/Components/DrawComponents/DrawAnimatedComponent.h
        Source/Actors/Tile.cpp
        Source/Actors/Tile.h
        Source/Actors/Cursor.cpp
        Source/Actors/Cursor.h
        Source/Actors/Unit.cpp
        Source/Actors/Unit.h
        Source/UIElements/UIScreen.cpp
        Source/UIElements/UIScreen.h
        Source/UIElements/UIFont.cpp
        Source/UIElements/UIFont.h
        Source/UIElements/UIButton.cpp
        Source/UIElements/UIButton.h
        Source/UIElements/UIText.cpp
        Source/UIElements/UIText.h
        Source/UIElements/UIElement.cpp
        Source/UIElements/UIElement.h
        Source/UIElements/UIImage.cpp
        Source/UIElements/UIImage.h
)

target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
)