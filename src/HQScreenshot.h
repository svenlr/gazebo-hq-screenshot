
#pragma once

#include <gazebo/common/Plugin.hh>
#include <gazebo/gui/GuiPlugin.hh>
#include <gazebo/common/Event.hh>
#include <gazebo/common/Events.hh>

#include "RenderToTexture.h"

#ifndef Q_MOC_RUN


#endif

namespace Ogre {
    class Camera;
    class SceneManager;
}

namespace gazebo {
    class GAZEBO_VISIBLE HQScreenshot : public GUIPlugin {
    Q_OBJECT

    public:
        HQScreenshot();

        virtual ~HQScreenshot();

    protected slots:

        void button_callback();

    private:
        Ogre::SceneManager *scene_manager{};
        RenderToTexture rtt_img{};
        Ogre::Camera *client_camera{};
        boost::shared_ptr<gazebo::event::Connection> m_render_subscription;

    private:
        void pre_render_callback();

        std::pair<std::string, Ogre::Camera *> search_camera(const std::string &camera_name);

        Ogre::SceneManager *get_scene_manager();

        void iterateSceneEntities(std::function<void(Ogre::Entity *)> &&func);

        void
        _iterateSceneRecursively(Ogre::SceneNode *root, std::function<void(Ogre::SceneNode *, int)> &func, int level);

        void iterateSceneRecursively(Ogre::SceneNode *root, std::function<void(Ogre::SceneNode *, int)> &&func);

    };
}
