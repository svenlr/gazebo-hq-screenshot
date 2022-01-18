
#pragma once

#include <gazebo/common/Plugin.hh>
#include <gazebo/gui/GuiPlugin.hh>
#include <gazebo/common/Event.hh>
#include <gazebo/common/Events.hh>

#include "RenderToTexture.h"


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
        /**
         * called each time before a render update in Gazebo
         */
        void pre_render_callback();

        /**
         * Find a pointer to camera by (partial) name.
         * Returns the first camera that contains the given string in its name.
         */
        std::pair<std::string, Ogre::Camera *> search_camera(const std::string &camera_name);

        /**
         * get the scene manager singleton instance
         */
        Ogre::SceneManager *get_scene_manager();

    };
}
