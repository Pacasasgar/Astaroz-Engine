#include "ModuleEditorCamera.h"


ModuleEditorCamera::ModuleEditorCamera()
{

}


ModuleEditorCamera::~ModuleEditorCamera()
{

}

bool ModuleEditorCamera::Init() 
{
	//Calculate aspect ratio
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	float aspect = w / h;

	//Frustum
	myFrustum.type = FrustumType::PerspectiveFrustum;
	myFrustum.pos = float3::zero;
	myFrustum.front = -float3::unitZ;
	myFrustum.up = float3::unitY;
	myFrustum.nearPlaneDistance = 0.1f;
	myFrustum.farPlaneDistance = 100.0f;
	myFrustum.verticalFov = math::pi / 4.0f;
	myFrustum.horizontalFov = 2.f * atanf(tanf(myFrustum.verticalFov * 0.5f) * aspect);

	//Matrices
	proj = myFrustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f), float3x3::RotateY(math::pi / 4.0f), float3(1.0f, 1.0f, 1.0f));
	view = LookAt(float3(0.0f, 1.f, 4.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));

	return true;
}

update_status ModuleEditorCamera::PreUpdate() 
{
	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::Update()
{
	//Grid
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
	
	//Axis
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	// red X
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
	glLineWidth(1.0f);
	
	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::PostUpdate() 
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditorCamera::CleanUp() 
{
	return true;
}

float4x4 ModuleEditorCamera::LookAt(float3 eye, float3 target, float3 up) 
{
	float4x4 matrix;
	math::float3 f(target - eye);
	f.Normalize();
	math::float3 s(f.Cross(up));
	s.Normalize();
	math::float3 u(s.Cross(f));
	matrix[0][0] = s.x;
	matrix[0][1] = s.y;
	matrix[0][2] = s.z;
	matrix[1][0] = u.x;
	matrix[1][1] = u.y;
	matrix[1][2] = u.z;
	matrix[2][0] = -f.x;
	matrix[2][1] = -f.y;
	matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye);
	matrix[1][3] = -u.Dot(eye);
	matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;

	return matrix;
}

void ModuleEditorCamera::changeFOV(float fov)
{
	//Calculate aspect ratio
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	float aspect = w / h;

	//Change FOV
	myFrustum.verticalFov = fov;
	myFrustum.horizontalFov = 2.f * atanf(tanf(myFrustum.verticalFov * 0.5f) * aspect);

	//Reasign projection matrix
	proj = myFrustum.ProjectionMatrix();
}

void ModuleEditorCamera::changePosition(float position)
{
	myFrustum.pos.x = position;
}