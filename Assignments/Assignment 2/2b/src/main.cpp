// This template code was originally written by Matt Overby while a TA for CSci5607

// The loaders are included by glfw3 (glcorearb.h) if we are not using glew.
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// Includes
#include "trimesh.hpp"
#include "shader.hpp"
#include <cstring> // memcpy

// Constants
#define WIN_WIDTH 500
#define WIN_HEIGHT 500

class Mat4x4 {
public:

	float m[16];

	Mat4x4(){ // Default: Identity
		m[0] = 1.f;  m[4] = 0.f;  m[8]  = 0.f;  m[12] = 0.f;
		m[1] = 0.f;  m[5] = 1.f;  m[9]  = 0.f;  m[13] = 0.f;
		m[2] = 0.f;  m[6] = 0.f;  m[10] = 1.f;  m[14] = 0.f;
		m[3] = 0.f;  m[7] = 0.f;  m[11] = 0.f;  m[15] = 1.f;
	}

	void make_identity(){
		m[0] = 1.f;  m[4] = 0.f;  m[8]  = 0.f;  m[12] = 0.f;
		m[1] = 0.f;  m[5] = 1.f;  m[9]  = 0.f;  m[13] = 0.f;
		m[2] = 0.f;  m[6] = 0.f;  m[10] = 1.f;  m[14] = 0.f;
		m[3] = 0.f;  m[7] = 0.f;  m[11] = 0.f;  m[15] = 1.f;
	}

	void print(){
		std::cout << m[0] << ' ' <<  m[4] << ' ' <<  m[8]  << ' ' <<  m[12] << "\n";
		std::cout << m[1] << ' ' <<   m[5] << ' ' <<  m[9]  << ' ' <<   m[13] << "\n";
		std::cout << m[2] << ' ' <<   m[6] << ' ' <<  m[10] << ' ' <<   m[14] << "\n";
		std::cout << m[3] << ' ' <<   m[7] << ' ' <<  m[11] << ' ' <<   m[15] << "\n";
	}
			
	void make_scale(float x, float y, float z){
		make_identity();
		m[0] = x; m[5] = y; m[10] = z;
	}

	void make_rotation(float degrees, char axis) {
		make_identity();
		float theta = degrees * 4 * atan(1) / 180;
		switch (axis) {
		case 'x': m[5] = cos(theta); m[6] = sin(theta); m[9] = -sin(theta); m[10] = cos(theta); break;
		case 'y': m[0] = cos(theta); m[2] = -sin(theta); m[8] = sin(theta); m[10] = cos(theta); break;
		case 'z': m[0] = cos(theta); m[1] = sin(theta); m[4] = -sin(theta); m[5] = cos(theta); break;
		default: m[0] = cos(theta); m[1] = sin(theta); m[4] = -sin(theta); m[5] = cos(theta); break;
		}
	}

	void make_translation(Vec3f movement) {
		make_identity();
		m[12] = movement[0];
		m[13] = movement[1];
		m[14] = movement[2];
	}
};

static inline const Vec3f operator*(const Mat4x4& m, const Vec3f& v) {
	Vec3f r(m.m[0] * v[0] + m.m[4] * v[1] + m.m[8] * v[2],
		m.m[1] * v[0] + m.m[5] * v[1] + m.m[9] * v[2],
		m.m[2] * v[0] + m.m[6] * v[1] + m.m[10] * v[2]);
	return r;
}

static inline const Vec3f operator+(const Vec3f& v1, const Vec3f& v2) {
	return Vec3f(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

static inline const Vec3f operator*(const Vec3f& v, const float& s) {
	return Vec3f(v[0] * s, v[1] * s, v[2] * s);
}

static inline const Vec3f operator*(const float& s, const Vec3f& v) {
	return Vec3f(v[0] * s, v[1] * s, v[2] * s);
}

static inline const Mat4x4 operator*(const Mat4x4& m1, const Mat4x4& m2) {
	Mat4x4 m3;

	m3.m[0] = (m1.m[0] * m2.m[0] + m1.m[4] * m2.m[1] + m1.m[8] * m2.m[2] + m1.m[12] * m2.m[3]);
	m3.m[1] = (m1.m[1] * m2.m[0] + m1.m[5] * m2.m[1] + m1.m[9] * m2.m[2] + m1.m[13] * m2.m[3]);
	m3.m[2] = (m1.m[2] * m2.m[0] + m1.m[6] * m2.m[1] + m1.m[10] * m2.m[2] + m1.m[14] * m2.m[3]);
	m3.m[3] = (m1.m[3] * m2.m[0] + m1.m[7] * m2.m[1] + m1.m[11] * m2.m[2] + m1.m[15] * m2.m[3]);

	m3.m[4] = (m1.m[0] * m2.m[4] + m1.m[4] * m2.m[5] + m1.m[8] * m2.m[6] + m1.m[12] * m2.m[7]);
	m3.m[5] = (m1.m[1] * m2.m[4] + m1.m[5] * m2.m[5] + m1.m[9] * m2.m[6] + m1.m[13] * m2.m[7]);
	m3.m[6] = (m1.m[2] * m2.m[4] + m1.m[6] * m2.m[5] + m1.m[10] * m2.m[6] + m1.m[14] * m2.m[7]);
	m3.m[7] = (m1.m[3] * m2.m[4] + m1.m[7] * m2.m[5] + m1.m[11] * m2.m[6] + m1.m[15] * m2.m[7]);

	m3.m[8] = (m1.m[0] * m2.m[8] + m1.m[4] * m2.m[9] + m1.m[8] * m2.m[10] + m1.m[12] * m2.m[11]);
	m3.m[9] = (m1.m[1] * m2.m[8] + m1.m[5] * m2.m[9] + m1.m[9] * m2.m[10] + m1.m[13] * m2.m[11]);
	m3.m[10] = (m1.m[2] * m2.m[8] + m1.m[6] * m2.m[9] + m1.m[10] * m2.m[10] + m1.m[14] * m2.m[11]);
	m3.m[11] = (m1.m[3] * m2.m[8] + m1.m[7] * m2.m[9] + m1.m[11] * m2.m[10] + m1.m[15] * m2.m[11]);

	m3.m[12] = (m1.m[0] * m2.m[12] + m1.m[4] * m2.m[13] + m1.m[8] * m2.m[14] + m1.m[12] * m2.m[15]);
	m3.m[13] = (m1.m[1] * m2.m[12] + m1.m[5] * m2.m[13] + m1.m[9] * m2.m[14] + m1.m[13] * m2.m[15]);
	m3.m[14] = (m1.m[2] * m2.m[12] + m1.m[6] * m2.m[13] + m1.m[10] * m2.m[14] + m1.m[14] * m2.m[15]);
	m3.m[15] = (m1.m[3] * m2.m[12] + m1.m[7] * m2.m[13] + m1.m[11] * m2.m[14] + m1.m[15] * m2.m[15]);

	return m3;
}

//
//	Global state variables
//
namespace Globals {
	double cursorX, cursorY; // cursor positions
	float win_width, win_height; // window size
	float aspect = WIN_WIDTH / WIN_HEIGHT;
	float pi = 4 * atan(1);
	float vfov = 90;
	float far = 60;
	float near = 0.5;
	GLuint verts_vbo[1], colors_vbo[1], normals_vbo[1], faces_ibo[1], tris_vao;
	TriMesh mesh;

	//  Model, view and projection matrices, initialized to the identity
	Mat4x4 model; // not used in this assignment; included for completeness only
	Mat4x4 view;
	Mat4x4 projection;

	Vec3f viewdir;
	Vec3f updir;
	Vec3f pos;
}

// Make a view matrix based on camera coordinate system
Mat4x4 makeView() {
	Mat4x4 mat;
	mat.make_identity();
	Vec3f u = Globals::viewdir.cross(Globals::updir);
	Vec3f v = u.cross(Globals::viewdir);
	Vec3f n = Globals::viewdir * -1;
	float d0 = -1 * u.dot(Globals::pos);
	float d1 = -1 * v.dot(Globals::pos);
	float d2 = -1 * n.dot(Globals::pos);
	Vec3f d = Vec3f(d0, d1, d2);
	mat.m[0] = u[0]; mat.m[4] = u[1]; mat.m[8] = u[2];  mat.m[12] = d[0];
	mat.m[1] = v[0]; mat.m[5] = v[1]; mat.m[9] = v[2];  mat.m[13] = d[1];
	mat.m[2] = n[0]; mat.m[6] = n[1]; mat.m[10] = n[2]; mat.m[14] = d[2];
	return mat;
}

// Make a perspective projection matrix
Mat4x4 makePerspective() {
	Mat4x4 mat;
	mat.make_identity();
	mat.m[0] = atan(Globals::vfov / 2) / 2;
	mat.m[5] = atan(Globals::vfov / 2);
	mat.m[10] = -1 * (Globals::far + Globals::near) / (Globals::far - Globals::near);
	mat.m[11] = -1;
	mat.m[14] = -2 * (Globals::far * Globals::near) / (Globals::far - Globals::near);
	mat.m[15] = 0;
	return mat;
}

//
//	Callbacks
//
static void error_callback(int error, const char* description){ fprintf(stderr, "Error: %s\n", description); }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	// Close on escape or Q
	if( action == GLFW_PRESS ){
		Mat4x4 m;
		Vec3f dir;
		float theta = 10;
		float travel = 1;
		switch ( key ) {
			case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
			case GLFW_KEY_Q: glfwSetWindowShouldClose(window, GL_TRUE); break;

            // Move the camera
			case GLFW_KEY_W: dir = Globals::viewdir;							dir.normalize(); Globals::pos = Globals::pos + travel * dir; break;
			case GLFW_KEY_A: dir = -1 * Globals::viewdir.cross(Globals::updir); dir.normalize(); Globals::pos = Globals::pos + travel * dir; break;
			case GLFW_KEY_S: dir = -1 * Globals::viewdir;						dir.normalize(); Globals::pos = Globals::pos + travel * dir; break;
			case GLFW_KEY_D: dir = Globals::viewdir.cross(Globals::updir);		dir.normalize(); Globals::pos = Globals::pos + travel * dir; break;
			case GLFW_KEY_LEFT_BRACKET:  dir = Vec3f(0, -1, 0);					dir.normalize(); Globals::pos = Globals::pos + travel * dir; break;
			case GLFW_KEY_RIGHT_BRACKET: dir = Vec3f(0, 1, 0);					dir.normalize(); Globals::pos = Globals::pos + travel * dir; break;

			// Turn the camera
			case GLFW_KEY_LEFT: m.make_rotation(theta, 'y');	   Globals::viewdir = m * Globals::viewdir; break;
			case GLFW_KEY_RIGHT: m.make_rotation(-1 * theta, 'y'); Globals::viewdir = m * Globals::viewdir; break;
			case GLFW_KEY_UP: m.make_rotation(theta, 'z');         Globals::viewdir = m * Globals::viewdir; Globals::updir = m * Globals::updir; break;
			case GLFW_KEY_DOWN: m.make_rotation(-1 * theta, 'z');  Globals::viewdir = m * Globals::viewdir; Globals::updir = m * Globals::updir; break;
		}
		Globals::view = makeView();
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	Globals::win_width = float(width);
	Globals::win_height = float(height);
    Globals::aspect = Globals::win_width/Globals::win_height;
	
    glViewport(0,0,width,height);

	// Update the perspective matrix as the window size changes
	Globals::projection = makePerspective();
}


// Function to set up geometry
void init_scene();


//
//	Main
//
int main(int argc, char *argv[]){

	// Load the mesh
	std::stringstream obj_file; obj_file << MY_DATA_DIR << "sibenik/sibenik.obj";
	if( !Globals::mesh.load_obj( obj_file.str() ) ){ return 0; }

	// Initialize viewing and projection matrices
	Globals::viewdir = Vec3f(1, 0, 0);
	Globals::updir = Vec3f(0, 1, 0);
	Globals::pos = Vec3f(0, -14, 0);
	Globals::view = makeView();

	Globals::projection = makePerspective();

	// Set up the window variable
	GLFWwindow* window;
    
    // Define the error callback function
	glfwSetErrorCallback(&error_callback);

	// Initialize glfw
	if( !glfwInit() ){ return EXIT_FAILURE; }

	// Ask for OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the glfw window
	Globals::win_width = WIN_WIDTH;
	Globals::win_height = WIN_HEIGHT;
	window = glfwCreateWindow(int(Globals::win_width), int(Globals::win_height), "HW2b", NULL, NULL);
	if( !window ){ glfwTerminate(); return EXIT_FAILURE; }

	// Define callbacks to handle user input and window resizing
	glfwSetKeyCallback(window, &key_callback);
	glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);

	// More setup stuff
	glfwMakeContextCurrent(window); // Make the window current
    glfwSwapInterval(1); // Set the swap interval

	// make sure the openGL and GLFW code can be found
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to gladLoadGLLoader" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Initialize the shaders
	// MY_SRC_DIR was defined in CMakeLists.txt
	// it specifies the full path to this project's src/ directory.
	mcl::Shader shader;
	std::stringstream ss; ss << MY_SRC_DIR << "shader.";
	shader.init_from_files( ss.str()+"vert", ss.str()+"frag" );

	// Initialize the scene
	init_scene();
	framebuffer_size_callback(window, int(Globals::win_width), int(Globals::win_height)); 

	// Perform some OpenGL initializations
	glEnable(GL_DEPTH_TEST);  // turn hidden surfce removal on
	glClearColor(1.f,1.f,1.f,1.f);  // set the background to white

	// Enable the shader, this allows us to set uniforms and attributes
	shader.enable();

	// Bind buffers
	glBindVertexArray(Globals::tris_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Globals::faces_ibo[0]);
    
	// Game loop
	while( !glfwWindowShouldClose(window) ){

		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Send updated info to the GPU
		glUniformMatrix4fv( shader.uniform("model"), 1, GL_FALSE, Globals::model.m  ); // model transformation (always the identity matrix in this assignment)
		glUniformMatrix4fv( shader.uniform("view"), 1, GL_FALSE, Globals::view.m  ); // viewing transformation
		glUniformMatrix4fv( shader.uniform("projection"), 1, GL_FALSE, Globals::projection.m ); // projection matrix

		// Draw
		glDrawElements(GL_TRIANGLES, Globals::mesh.faces.size()*3, GL_UNSIGNED_INT, 0);

		// Finalize
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // end game loop

	// Unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Disable the shader, we're done using it
	shader.disable();
    
	return EXIT_SUCCESS;
}


void init_scene(){

	using namespace Globals;

	// Create the buffer for vertices
	glGenBuffers(1, verts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, verts_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size()*sizeof(mesh.vertices[0]), &mesh.vertices[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create the buffer for colors
	glGenBuffers(1, colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh.colors.size()*sizeof(mesh.colors[0]), &mesh.colors[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create the buffer for normals
	glGenBuffers(1, normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh.normals.size()*sizeof(mesh.normals[0]), &mesh.normals[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create the buffer for indices
	glGenBuffers(1, faces_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size()*sizeof(mesh.faces[0]), &mesh.faces[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create the VAO
	glGenVertexArrays(1, &tris_vao);
	glBindVertexArray(tris_vao);

	int vert_dim = 3;

	// location=0 is the vertex
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, verts_vbo[0]);
	glVertexAttribPointer(0, vert_dim, GL_FLOAT, GL_FALSE, sizeof(mesh.vertices[0]), 0);

	// location=1 is the color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo[0]);
	glVertexAttribPointer(1, vert_dim, GL_FLOAT, GL_FALSE, sizeof(mesh.colors[0]), 0);

	// location=2 is the normal
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo[0]);
	glVertexAttribPointer(2, vert_dim, GL_FLOAT, GL_FALSE, sizeof(mesh.normals[0]), 0);

	// Done setting data for the vao
	glBindVertexArray(0);

}

