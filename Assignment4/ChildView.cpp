
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "Assignment4.h"
#include "ChildView.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CShaderWnd)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CShaderWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::InitGL()
{
	glClearColor(0.2f, 0.6f, 0.4f, 1.f);
	glEnable(GL_DEPTH_TEST);

	m_bunnyTex.LoadFile(L"models/bunny-atlas.jpg");

	m_bunny.LoadOBJ("models\\bunny.obj");
	m_bunny.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_bunny.InitGL();

	m_fishTex.LoadFile(L"models/BLUEGILL.bmp");
	m_fish.LoadOBJ("models\\fish4.obj");
	m_fish.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_fish.InitGL();

	m_prismTex.LoadFile(L"textures/plank01.bmp");
	const int prismSides = 10;
	const float prismRadius = 2.0f;
	const float prismHalfHeight = 1.5f;
	const float kPi = 3.14159265358979323846f;
	int vIndex = 0;
	int nIndex = 0;
	int tIndex = 0;

	// Side faces (each quad split into two triangles)
	for (int i = 0; i < prismSides; i++)
	{
		float a0 = 2.0f * kPi * (float)i / (float)prismSides;
		float a1 = 2.0f * kPi * (float)(i + 1) / (float)prismSides;
		float u0 = (float)i / (float)prismSides;
		float u1 = (float)(i + 1) / (float)prismSides;

		m_prism.AddVertex(vec3(prismRadius * cosf(a0), -prismHalfHeight, prismRadius * sinf(a0))); // v0
		m_prism.AddVertex(vec3(prismRadius * cosf(a0), prismHalfHeight, prismRadius * sinf(a0)));  // v1
		m_prism.AddVertex(vec3(prismRadius * cosf(a1), prismHalfHeight, prismRadius * sinf(a1)));  // v2
		m_prism.AddVertex(vec3(prismRadius * cosf(a1), -prismHalfHeight, prismRadius * sinf(a1))); // v3

		float am = 0.5f * (a0 + a1);
		m_prism.AddNormal(vec3(cosf(am), 0.f, sinf(am)));

		m_prism.AddTexCoord(vec2(u0, 0.f));
		m_prism.AddTexCoord(vec2(u0, 1.f));
		m_prism.AddTexCoord(vec2(u1, 1.f));
		m_prism.AddTexCoord(vec2(u1, 0.f));

		m_prism.AddTriangleVertex(vIndex + 0, nIndex, tIndex + 0);
		m_prism.AddTriangleVertex(vIndex + 1, nIndex, tIndex + 1);
		m_prism.AddTriangleVertex(vIndex + 2, nIndex, tIndex + 2);
		m_prism.AddTriangleVertex(vIndex + 0, nIndex, tIndex + 0);
		m_prism.AddTriangleVertex(vIndex + 2, nIndex, tIndex + 2);
		m_prism.AddTriangleVertex(vIndex + 3, nIndex, tIndex + 3);

		vIndex += 4;
		nIndex += 1;
		tIndex += 4;
	}

	// Top cap
	m_prism.AddNormal(vec3(0.f, 1.f, 0.f));
	int topNormalIndex = nIndex++;
	for (int i = 0; i < prismSides; i++)
	{
		float a0 = 2.0f * kPi * (float)i / (float)prismSides;
		float a1 = 2.0f * kPi * (float)(i + 1) / (float)prismSides;

		m_prism.AddVertex(vec3(0.f, prismHalfHeight, 0.f));
		m_prism.AddVertex(vec3(prismRadius * cosf(a0), prismHalfHeight, prismRadius * sinf(a0)));
		m_prism.AddVertex(vec3(prismRadius * cosf(a1), prismHalfHeight, prismRadius * sinf(a1)));

		m_prism.AddTexCoord(vec2(0.5f, 0.5f));
		m_prism.AddTexCoord(vec2(0.5f + 0.5f * cosf(a0), 0.5f + 0.5f * sinf(a0)));
		m_prism.AddTexCoord(vec2(0.5f + 0.5f * cosf(a1), 0.5f + 0.5f * sinf(a1)));

		m_prism.AddTriangleVertex(vIndex + 0, topNormalIndex, tIndex + 0);
		m_prism.AddTriangleVertex(vIndex + 1, topNormalIndex, tIndex + 1);
		m_prism.AddTriangleVertex(vIndex + 2, topNormalIndex, tIndex + 2);

		vIndex += 3;
		tIndex += 3;
	}

	// Bottom cap
	m_prism.AddNormal(vec3(0.f, -1.f, 0.f));
	int bottomNormalIndex = nIndex++;
	for (int i = 0; i < prismSides; i++)
	{
		float a0 = 2.0f * kPi * (float)i / (float)prismSides;
		float a1 = 2.0f * kPi * (float)(i + 1) / (float)prismSides;

		m_prism.AddVertex(vec3(0.f, -prismHalfHeight, 0.f));
		m_prism.AddVertex(vec3(prismRadius * cosf(a1), -prismHalfHeight, prismRadius * sinf(a1)));
		m_prism.AddVertex(vec3(prismRadius * cosf(a0), -prismHalfHeight, prismRadius * sinf(a0)));

		m_prism.AddTexCoord(vec2(0.5f, 0.5f));
		m_prism.AddTexCoord(vec2(0.5f + 0.5f * cosf(a1), 0.5f + 0.5f * sinf(a1)));
		m_prism.AddTexCoord(vec2(0.5f + 0.5f * cosf(a0), 0.5f + 0.5f * sinf(a0)));

		m_prism.AddTriangleVertex(vIndex + 0, bottomNormalIndex, tIndex + 0);
		m_prism.AddTriangleVertex(vIndex + 1, bottomNormalIndex, tIndex + 1);
		m_prism.AddTriangleVertex(vIndex + 2, bottomNormalIndex, tIndex + 2);

		vIndex += 3;
		tIndex += 3;
	}

	m_prism.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_prism.InitGL();

	m_cat.LoadOBJ("models\\cat.obj");
	m_cat.m_program = LoadShaders("ShaderWnd/vertexSphere2.glsl", "ShaderWnd/fragmentSphere2.glsl");
	m_cat.InitGL();

	m_heightTex.LoadFile(L"textures/height.bmp");
	m_sphereTex.LoadFile(L"textures/bumpmap.jpg");
	m_sphere.SetRadius(3);
	m_sphere.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/parallaxSphere.glsl");
	m_sphere.InitGL();

	m_cubeTex.LoadFiles(L"textures/right.jpg", L"textures/left.jpg", L"textures/top.jpg",
		L"textures/bottom.jpg", L"textures/front.jpg", L"textures/back.jpg");

	m_skybox.CreateCube();
	m_skybox.m_program = LoadShaders("ShaderWnd/vertexSky.glsl", "ShaderWnd/fragmentSky.glsl");
	m_skybox.InitGL();
}

void CChildView::RenderGL()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program = m_bunny.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "diffuse_mat"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(m_mPVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(m_mVM));

	glm::vec4 light_position(-5.f, 5.f, -5.f, 0.f);
	glm::vec4 light_ambient(0.2f, 0.2f, 0.2f, 1.f);
	glm::vec4 light_diffuse(1.f, 1.f, 1.f, 1.f);
	glm::vec4 light_specular(1.f, 1.f, 1.f, 1.f);

	glm::vec4 material_ambient(1.f, 1.f, 1.f, 1.f);
	glm::vec4 material_diffuse(1.f, 1.f, 1.f, 1.f);
	glm::vec4 material_specular(1.f, 1.f, 1.f, 1.f);
	float material_shininess = 100.0f;

	glm::vec4 ambient_product = light_ambient * material_ambient;
	glm::vec4 diffuse_product = light_diffuse * material_diffuse;
	glm::vec4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_bunnyTex.TexName());

	m_bunny.RenderGL();

	m_program = m_sphere.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);
	glUniform1i(glGetUniformLocation(m_program, "height_map"), 1);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M = translate(mat4(1.f), vec3(-10., 0., 0.))
		* rotate(mat4(1.f), 90.f, vec3(0., 1., 0.));
	glm::mat4 VM = m_mVM * M;
	glm::mat4 PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sphereTex.TexName());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_heightTex.TexName());

	m_sphere.RenderGL();

	m_program = m_cat.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "env_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(12.f, -0.8f, -1.5f))
		* rotate(mat4(1.f), 180.f, vec3(0.f, 1.f, 0.f))
		* scale(mat4(1.f), vec3(0.35f));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTex.TexName());
	m_cat.RenderGL();

	m_program = m_fish.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "diffuse_mat"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(12.f, -1.2f, 2.2f))
		* rotate(mat4(1.f), -30.f, vec3(0.f, 1.f, 0.f))
		* scale(mat4(1.f), vec3(0.7f));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));

	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fishTex.TexName());
	m_fish.RenderGL();

	m_program = m_prism.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "diffuse_mat"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(-4.f, -0.2f, 8.f))
		* rotate(mat4(1.f), 18.f, vec3(0.f, 1.f, 0.f))
		* scale(mat4(1.f), vec3(0.85f));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));

	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_prismTex.TexName());
	m_prism.RenderGL();

	m_program = m_skybox.m_program;
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "skybox"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mP");
	m_nVM = glGetUniformLocation(m_program, "mV");
	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(m_mProjection));
	glm::mat4 view = glm::mat4(glm::mat3(m_mVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(view));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTex.TexName());
	glDepthFunc(GL_LEQUAL);
	m_skybox.RenderGL();
	glDepthFunc(GL_LESS);
}

void CChildView::CleanGL()
{
	m_bunny.CleanGL();
	m_bunnyTex.Clear();
	m_fish.CleanGL();
	m_fishTex.Clear();
	m_prism.CleanGL();
	m_prismTex.Clear();
	m_cat.CleanGL();
	m_sphere.CleanGL();
	m_sphereTex.Clear();
	m_heightTex.Clear();
	m_skybox.CleanGL();
	m_cubeTex.Clear();
}
