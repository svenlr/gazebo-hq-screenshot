#include "HQScreenshot.h"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include "png_utils.h"

using namespace gazebo;

GZ_REGISTER_GUI_PLUGIN(HQScreenshot)

HQScreenshot::HQScreenshot()
        : GUIPlugin() {
    QHBoxLayout *mainLayout = new QHBoxLayout;
    QFrame *mainFrame = new QFrame();
    QVBoxLayout *frameLayout = new QVBoxLayout();
    QPushButton *button = new QPushButton(tr("HQ Screenshot"));
    connect(button, SIGNAL(clicked()), this, SLOT(button_callback()));
    frameLayout->addWidget(button);
    mainFrame->setLayout(frameLayout);
    mainLayout->addWidget(mainFrame);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    frameLayout->setContentsMargins(0, 0, 0, 0);

    this->setStyleSheet("QFrame { background-color : rgba(100, 100, 100, 255); color : white; }");
    this->setLayout(mainLayout);
    this->move(5, 5);
    this->resize(130, 40);

    m_render_subscription = event::Events::ConnectPreRender(
            boost::bind(&HQScreenshot::pre_render_callback, this));
}

HQScreenshot::~HQScreenshot() {
}

void HQScreenshot::button_callback() {
    if (rtt_img.rendering_texture) {
        auto *img = new uchar[rtt_img.width * rtt_img.height * 3];
        rtt_img.render_to_buffer(img);
        std::stringstream concat;
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        concat << std::getenv("HOME") << "/.gazebo/pictures/";
        concat << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S") << ".png";
        write_rgb_png(concat.str(), img, rtt_img.width, rtt_img.height);
        delete[] img;
    } else {
        std::cerr << "HQ Screenshot: " << " initialization failed" << std::endl;
    }
}

std::pair<std::string, Ogre::Camera *> HQScreenshot::search_camera(const std::string &camera_name) {
    const auto &cameras = scene_manager->getCameras();
    for (const auto &camera : cameras) {
        if (camera.first.find(camera_name) != std::string::npos) {
            return camera;
        }
    }
    return {};
}

void HQScreenshot::pre_render_callback() {
    if (scene_manager) {
        if (not client_camera) {
            auto ret = search_camera("client");
            std::cerr << "camera: " << ret.first << std::endl;
            client_camera = ret.second;
        }
        if (client_camera and not rtt_img.rendering_texture) {
            client_camera->setRenderingMinPixelSize(0);
            client_camera->setUseRenderingDistance(false);
            client_camera->setPolygonMode(Ogre::PolygonMode::PM_SOLID);
            client_camera->setLodBias(2.0);
            iterateSceneEntities([&](Ogre::Entity *obj) {
                bool is_floor = false;
                for (int k = 0; k < obj->getNumSubEntities(); ++k) {
                    auto sub_entity = obj->getSubEntity(k);
                    const auto &mat = sub_entity->getMaterial();
                    if (mat->getName().find("Surface") != std::string::npos) {
                        is_floor = true;
                    }
                }
                if (not is_floor) {
                    obj->setRenderQueueGroup(3);
                }
            });
            int w = client_camera->getViewport()->getActualWidth();
            int h = client_camera->getViewport()->getActualHeight();
            rtt_img.initialize("hq_screenshot", client_camera, w, h);
        }
    } else { // scene_manager == nullptr
        scene_manager = get_scene_manager();
    }
}

Ogre::SceneManager *HQScreenshot::get_scene_manager() {
    if (not Ogre::Root::getSingletonPtr()) {
        std::cout << "Ogre::Root::getSingletonPtr() is nullptr" << std::endl;
        return nullptr;
    }
    auto it = Ogre::Root::getSingletonPtr()->getSceneManagerIterator();
    if (it.begin() != it.end()) {
        return it.begin()->second;
    } else {
        return nullptr;
    }
}

void HQScreenshot::iterateSceneEntities(std::function<void(Ogre::Entity *)> &&func) {
    iterateSceneRecursively(
            scene_manager->getRootSceneNode(), [&, this](Ogre::SceneNode *node, int level) {
                auto it = node->getAttachedObjectIterator();
                while (it.hasMoreElements()) {
                    auto obj = dynamic_cast<Ogre::Entity *>(it.peekNextValue());
                    if (obj) {
                        func(obj);
                    }
                    it.moveNext();
                }
            });
}

void HQScreenshot::_iterateSceneRecursively(Ogre::SceneNode *root, std::function<void(Ogre::SceneNode *, int)> &func,
                                            int level) {
    func(root, level);
    auto it = root->getChildIterator();
    while (it.hasMoreElements()) {
        auto node = dynamic_cast<Ogre::SceneNode *> (it.peekNextValue());
        if (node) {
            _iterateSceneRecursively(node, func, level + 1);
        }
        it.moveNext();
    }
}

void HQScreenshot::iterateSceneRecursively(Ogre::SceneNode *root, std::function<void(Ogre::SceneNode *, int)> &&func) {
    _iterateSceneRecursively(root, func, 0);
}
