import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Qt3D.Core 2.15
import Qt3D.Render 2.15
import Qt3D.Input 2.15
import Qt3D.Extras 2.15
import QtQuick.Scene3D

Window {
    id: mainWindow
    width: 800
    height: 600
    visible: true
    title: qsTr("3D Куб")
    color: "#ffffff"

    Scene3D {
        id: scene3d
        anchors.fill: parent
        aspects: ["input", "logic"]

        Entity {
            id: rootEntity

            // Камера
            Camera {
                id: camera
                position: Qt.vector3d(0, 0, 10)
                viewCenter: Qt.vector3d(0, 0, 0)
                upVector: Qt.vector3d(0, 1, 0)
                aspectRatio: scene3d.width / scene3d.height
                nearPlane: 0.1
                farPlane: 1000.0
                fieldOfView: 45
            }

            // Настройка освещения и рендеринга
            components: [
                RenderSettings {
                    activeFrameGraph: ForwardRenderer {
                        clearColor: "#333333"  // Фон сцены: серо-чёрный
                        camera: camera
                    }
                },
                InputSettings { }
            ]

            // Источник света
            Entity {
                id: lightEntity

                // Источник света
                components: [
                    PointLight {
                        color: "#da1010"
                        intensity: 1.0
                    },
                    Transform {
                        translation: Qt.vector3d(0, 10, 0)  // Позиция источника света
                    }
                ]

                // Сфера для визуализации источника света
                Entity {
                    components: [
                        SphereMesh {
                            radius: 0.5  // Радиус сферы
                        },
                        PhongMaterial {
                            diffuse: "#da1010"  // Цвет источника
                            ambient: "#aa1010"
                        },
                        Transform {
                            translation: Qt.vector3d(0, 10, 0)
                        }
                    ]
                }

                // Эффект свечения вокруг источника света
                Entity {
                    components: [
                        SphereMesh {
                            radius: 1.2  // Радиус эффекта свечения
                        },
                        PhongMaterial {
                            diffuse: Qt.rgba(1.0, 0.4, 0.4, 0.3)  // Прозрачный красный цвет для свечения
                            ambient: "#ff0000"  // Цвет окружающего света
                            specular: "#ff0000"  // Цвет бликов
                            shininess: 5  // Плавное свечение
                        },
                        Transform {
                            translation: Qt.vector3d(0, 10, 0)  // Позиция совпадает с источником света
                        }
                    ]
                }
            }

            // Куб
            Entity {
                id: cubeEntity

                property real rotationX: 0
                property real rotationY: 0

                Transform {
                    id: cubeTransform
                    scale: 2.0
                    rotation: fromEulerAngles(cubeEntity.rotationX, cubeEntity.rotationY, 0)
                }

                CuboidMesh {
                    id: cubeMesh
                }

                PhongMaterial {
                    id: cubeMaterial
                    ambient: "#555555"
                    diffuse: "#888888"
                    specular: "#ffffff"
                    shininess: 100
                }

                components: [cubeMesh, cubeMaterial, cubeTransform]
            }

            // Обработчик ввода
            MouseHandler {
                id: mouseHandler
                sourceDevice: MouseDevice {}

                property point lastPosition
                property bool leftButtonPressed: false
                property bool rightButtonPressed: false

                onPressed: function(mouse) {
                    lastPosition = Qt.point(mouse.x, mouse.y)
                    if (mouse.button === Qt.LeftButton) {
                        leftButtonPressed = true
                    } else if (mouse.button === Qt.RightButton) {
                        rightButtonPressed = true
                    }
                }

                onReleased: function(mouse) {
                    if (mouse.button === Qt.LeftButton) {
                        leftButtonPressed = false
                    } else if (mouse.button === Qt.RightButton) {
                        rightButtonPressed = false
                    }
                }

                onPositionChanged: function(mouse) {
                    var dx = mouse.x - lastPosition.x
                    var dy = mouse.y - lastPosition.y

                    if (leftButtonPressed) {
                        // Движение камеры вокруг куба
                        var distance = camera.position.length()
                        var sensitivity = 0.5

                        // Углы Эйлера для вращения камеры
                        var theta = Math.atan2(camera.position.x, camera.position.z) + dx * 0.01 * sensitivity
                        var phi = Math.acos(camera.position.y / distance) + dy * 0.01 * sensitivity

                        // Ограничиваем phi для предотвращения переворота камеры
                        var phiClamped = Math.max(0.1, Math.min(Math.PI - 0.1, phi))

                        // Вычисляем новую позицию камеры в сферических координатах
                        var x = distance * Math.sin(phiClamped) * Math.sin(theta)
                        var y = distance * Math.cos(phiClamped)
                        var z = distance * Math.sin(phiClamped) * Math.cos(theta)

                        camera.position = Qt.vector3d(x, y, z)
                        camera.viewCenter = Qt.vector3d(0, 0, 0)
                    }

                    if (rightButtonPressed) {
                        // Вращение куба
                        cubeEntity.rotationY += dx * 0.5
                        cubeEntity.rotationX += dy * 0.5
                    }

                    lastPosition = Qt.point(mouse.x, mouse.y)
                }
            }
        }
    }
    Shortcut {
        sequence: "Esc"  // Назначение клавиши Esc
        onActivated: {
            Qt.quit()  // Выход из приложения
        }
    }

    Shortcut {
        sequence: "q"  // Назначение клавиши Esc
        onActivated: {
            number.text = number.text + '1'  // Выход из приложения
            if (number.text.length > 10) {
                number.text = '2'
            }
        }
    }
    Text {
        id: number
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            bottomMargin: 50
        }
        horizontalAlignment: Text.AlignHCenter
        text: "1"
    }
    Text {
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            bottomMargin: 20
        }
        color: "black"
        text: "Левая кнопка мыши: перемещение камеры\nПравая кнопка мыши: вращение куба"
        horizontalAlignment: Text.AlignHCenter
    }
}
