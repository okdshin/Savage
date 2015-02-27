#include <iostream>
#include <memory>
#include <savage/savage.hpp>

namespace sh = savage::shader;
namespace sr = savage::renderer;

sr::scene_node_transform_chain_generator const transform;

class kinematic_motion_state : public btMotionState {
public:
	kinematic_motion_state(btTransform const& initial_trans) : transform_(initial_trans) {}
	virtual ~kinematic_motion_state() {}
	virtual void getWorldTransform(btTransform& worldTrans) const {
		worldTrans = transform_;
	}
	void set_transform(btTransform const& trans) {
		transform_ = trans;
	}
	virtual void setWorldTransform(const btTransform &worldTrans) {
		// do nothing
	}

private:
	btTransform transform_;
};

class Application {
public:
	Application() : program_(),
		entity_manager_(
			program_, 
			savage::renderer::entity_info(
				savage::shader::attribute(0, "vec3", "VertexPosition"),
				savage::shader::attribute(1, "vec3", "VertexNormal"),
				savage::shader::attribute(2, "vec3", "VertexTexCoord"),
				savage::shader::uniform("sampler2D", "DiffuseTexture")
			)
		),
		scene_root_() {}
	
	void poll_input() {
		if(savage::get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(
				savage::window_manager::instance().window().handle(), GL_TRUE);
		}

		// camera zoom in/out
		{
			if(savage::get_key(GLFW_KEY_SPACE) == GLFW_PRESS) {
				camera_->set_angle(savage::pi<float>::value/16.f);
			}
			if(savage::get_key(GLFW_KEY_SPACE) == GLFW_RELEASE) {
				camera_->set_angle(savage::pi<float>::value/4.f);
			}
		}
	}

	void initialize() {
		clock_.init();
		physics_engine_.init();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		sh::attach<GL_VERTEX_SHADER>(program_, "vertex.shader");
		sh::attach<GL_FRAGMENT_SHADER>(program_, "frag.shader");
		entity_manager_.bind_attribute();
		sh::link(program_);
		sh::use(program_);

		//Cameraman
		{
			camera_man_ = scene_root_.create_child([this](sr::scene_node& sn){
				auto dt = static_cast<float>(savage::delta_second(clock_));
				{
					auto width = savage::window_manager::instance().window().width();
					auto height = savage::window_manager::instance().window().height();
					auto pos = savage::get_cursor_position();
					auto pitch = pos[0]/width-0.5f;
					auto yaw = pos[1]/height-0.5f;
					glfwSetCursorPos(savage::window_manager::instance().window().handle(),
						0.5f*width, 0.5f*height);
					sr::transform_chain(sn).rotate(0, -pitch, -yaw);
					
				}
				{
					auto camera_speed = 1.0f;
					auto trans = sn.chain(transform);
					if(savage::get_key(GLFW_KEY_W) == GLFW_PRESS) {
						trans.translate_local(dt*camera_speed*glm::vec3(0.f, 0.f, -1.f));
					}
					if(savage::get_key(GLFW_KEY_S) == GLFW_PRESS) {
						trans.translate_local(dt*camera_speed*glm::vec3(0.f, 0.f, 1.f));
					}
					if(savage::get_key(GLFW_KEY_A) == GLFW_PRESS) {
						trans.translate_local(dt*camera_speed*glm::vec3(-1.f, 0.f, 0.f));
					} 
					if(savage::get_key(GLFW_KEY_D) == GLFW_PRESS) {
						trans.translate_local(dt*camera_speed*glm::vec3(1.f, 0.f, 0.f));
					}
				}
			})->chain(transform).translate(0, 1.7f, 0).chain_end();

			//Camera
			{
				camera_ = std::make_unique<sr::camera>(
						program_,
						sh::uniform("mat4", "ProjectionMatrix"),
						sh::uniform("mat4", "ViewMatrix"),
						savage::pi<float>::value/4.f, 4.0f/3.0f, 0.1f, 1000.0f
					);
				camera_man_->create_child(
					sr::parent_transform_inheritor<sr::camera>(camera_.get()));
			}
		}

		//Skybox
		{
			auto entities = sr::load_obj(
				entity_manager_,
				"tex_cube/skybox.obj"
			);
			camera_man_->create_child(sr::skybox(
					program_, sh::uniform("mat4", "ModelMatrix"), *entities.at(0), 
					camera_man_))
				->chain(transform)
					.rotate(-savage::pi<float>::value/2.f,0,0)
					.scale_local(100.f);
		}

		//Box object
		{
			auto mass = 100.;
			btCollisionShape* box_shape;
			glm::vec3 inertia;
			std::tie(box_shape, inertia) = 
				shape_manager_.create_shape<btBoxShape>(mass, btVector3(1,1,1));
			auto entities = sr::load_obj(
				entity_manager_,
				"tex_cube/tex_cube.obj"
			);
			for(int i=0; i < 500; ++i) {
				savage::physics::collision_body collision_body(physics_engine_);
				auto motion_state = btDefaultMotionState(
					btTransform(
						btQuaternion(0.5, 0.5, 0.5*i, 1), 
						savage::physics::glm_to_bt(glm::vec3(0,2*i,-3))
					)
				);
				collision_body.init(box_shape, mass, inertia, motion_state);
				auto box = scene_root_.create_child(collision_body);
				box_objects_.emplace_back(std::make_unique<sr::object>(
					program_, sh::uniform("mat4", "ModelMatrix"), *entities.at(0)));
				box->create_child(sr::parent_transform_inheritor<sr::object>(
						box_objects_.back().get()
					))->chain(transform).scale_local(0.1, 1, 0.1);
			}
		}

		/*
		//Light
		{
			auto light = scene_root_.create_child(sr::point_light(
				program_, 
				sh::uniform("float", "Ka"),
				sh::uniform("float", "Kd"),
				sh::uniform("float", "Ks"),
				sh::uniform("vec3", "LightColor"),
				sh::uniform("vec3", "LightPosition"),
				1, 1, 1, glm::vec3(1.f, 1.0f, 0.8f))
			);
			sr::transform_chain(*light)
				//.rotate(0.f, savage::pi<float>::value/4.f, 0.f)
				.translate_local(0.f, 200.f, -2.f)
				;
		}
		*/
	}
	void update() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		poll_input();
		physics_engine_.update();
		scene_root_.project();
		clock_.update();
		std::cout << clock_.fps() << std::endl;
	}

private:
	savage::clock clock_;
	sh::program program_;
	sr::scene_node* camera_man_;
	std::unique_ptr<sr::camera> camera_;
	sr::entity_manager entity_manager_;
	sr::scene_node scene_root_;
	std::unique_ptr<sh::texture> texture_, skybox_texture_;
	savage::physics::engine physics_engine_;
	savage::physics::shape_manager shape_manager_;
	std::vector<std::unique_ptr<sr::object>> box_objects_;
};

int main() {
	std::cout << "hello world" << std::endl;
	try {
		savage::initialize("Savage_HelloWorld", 1280, 960, false);
		Application application;
		savage::register_app(application);
		application.initialize();
		savage::main_loop();
	}
	catch(std::exception const& e) {
		std::cerr << e.what() << std::endl;
	}
	glfwDestroyWindow(savage::window_manager::instance().window().handle());
	savage::terminate();
}
