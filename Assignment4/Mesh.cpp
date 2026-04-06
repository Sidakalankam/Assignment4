#include "pch.h"
#include <fstream>      // For input streams from files
#include <string>       // For the string type
#include <sstream>      // For streams from strings
#include <cstdlib>      // For atoi
#include "Mesh.h"
using namespace std;


CMesh::CMesh(void)
{
}


CMesh::~CMesh(void)
{
}


void CMesh::InitGL()
{
	vertexArray.clear();
	normalArray.clear();
	texArray.clear();

    for(PTV v=m_triangles.begin();  v!=m_triangles.end();  v++)
    {
		const TV &tv = *v;
		if(tv.v < 0 || tv.v >= int(m_vertices.size()))
			continue;

		vertexArray.push_back(m_vertices[tv.v]);

		if(tv.n >= 0 && tv.n < int(m_normals.size()))
			normalArray.push_back(m_normals[tv.n]);
		else
			normalArray.push_back(glm::vec3(0.f, 0.f, 1.f));

		if(tv.t >= 0 && tv.t < int(m_tvertices.size()))
			texArray.push_back(m_tvertices[tv.t]);
		else
			texArray.push_back(glm::vec2(0.f, 0.f));
	}

	if(vertexArray.empty())
		return;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size()*sizeof(glm::vec3), 
		&vertexArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &m_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
	glBufferData(GL_ARRAY_BUFFER, normalArray.size()*sizeof(glm::vec3),
		&normalArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &m_texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
	glBufferData(GL_ARRAY_BUFFER, texArray.size()*sizeof(glm::vec2),
		&texArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void CMesh::RenderGL()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexArray.size());
}

void CMesh::CleanGL()
{
	glDeleteBuffers(1, &m_vertexVBO);
	glDeleteBuffers(1, &m_normalVBO);
	glDeleteBuffers(1, &m_texVBO);
	glDeleteVertexArrays(1, &m_vao);
}

void CMesh::AddTriangleVertex(int v, int n, int t)
{
    TV tv;
    tv.v = v;
    tv.n = n;
    tv.t = t;
    m_triangles.push_back(tv);
}

//
// Name :         CMesh::AddFlatQuad()
// Description :  Add a quadrangle to the triangle mesh.
//
void CMesh::AddFlatQuad(int a, int b, int c, int d, int n)
{
	
	 // First triangle
    AddTriangleVertex(a, n, -1);
    AddTriangleVertex(b, n, -1);
    AddTriangleVertex(c, n, -1);

    // Second triangle
    AddTriangleVertex(a, n, -1);
    AddTriangleVertex(c, n, -1);
    AddTriangleVertex(d, n, -1);
}


void CMesh::AddQuad(int a, int b, int c, int d)
{
	 // First triangle
    AddTriangleVertex(a, a, -1);
    AddTriangleVertex(b, b, -1);
    AddTriangleVertex(c, c, -1);

    // Second triangle
    AddTriangleVertex(a, a, -1);
    AddTriangleVertex(c, c, -1);
    AddTriangleVertex(d, d, -1);
}


//
// Name :         CMesh::ComputeSmoothNormals()
// Description :  Compute a normal for each vertex that is the
//                average of the surface normals incident on the 
//                vertex.
//
void CMesh::ComputeSmoothNormals(void)
{
	m_normals.resize(m_vertices.size());
    for(unsigned int i=0;  i<m_vertices.size();  i++)
        m_normals[i] = glm::vec3(0, 0, 0);

	 for(PTV v=m_triangles.begin();  v!=m_triangles.end();  )
    {
        // Collect up the vertices of a triangle...
        int a = v->v;
        v++;
        int b = v->v;
        v++;
        int c = v->v;
        v++;

        // Surface normal
        glm::vec3 normal = glm::cross(m_vertices[b] - m_vertices[a], 
                                 m_vertices[c] - m_vertices[a]);
        normal *=1/normal.length();

        // Add to the incident vertices normals
        m_normals[a] += normal;
        m_normals[b] += normal;
        m_normals[c] += normal;

		 // Normalize the normals
    for(unsigned int i=0;  i<m_vertices.size();  i++)
        m_normals[i] *= 1/m_normals[i].length();
}
}

void CMesh::LoadOBJ(const char* filename)
{
	 ifstream str(filename);
    if(!str)
    {
		CString msg;
		msg.Format(L"File not found: %hs", filename);
        AfxMessageBox(msg);
        return;
    }

    string line;
    while(getline(str, line))
    {
        istringstream lstr(line);

        string code;
        lstr >> code;
        if(code == "v") 
        {
            double x, y, z;
            lstr >> x >> y >> z;
            AddVertex(glm::vec3(x, y, z));
        }
        else if(code == "vn")
        {
			double a, b, c;
            lstr >> a >> b >> c;
            AddNormal(glm::vec3(a, b, c));	
        }
        else if(code == "vt")
        {
			double s, t;
            lstr >> s >> t;
            AddTexCoord(glm::vec2(s, t));
        }
        else if(code == "f")
        {
			vector<TV> face;
			string token;
			while(lstr >> token)
			{
				TV tv;
				tv.v = -1;
				tv.t = -1;
				tv.n = -1;

				int part = 0;
				string cur;
				for(size_t i = 0; i <= token.size(); i++)
				{
					if(i == token.size() || token[i] == '/')
					{
						if(!cur.empty())
						{
							int idx = atoi(cur.c_str());
							if(idx > 0)
							{
								if(part == 0) tv.v = idx - 1;
								else if(part == 1) tv.t = idx - 1;
								else if(part == 2) tv.n = idx - 1;
							}
						}

						cur.clear();
						part++;
					}
					else
					{
						cur.push_back(token[i]);
					}
				}

				face.push_back(tv);
			}

			for(size_t i = 1; i + 1 < face.size(); i++)
			{
				AddTriangleVertex(face[0].v, face[0].n, face[0].t);
				AddTriangleVertex(face[i].v, face[i].n, face[i].t);
				AddTriangleVertex(face[i + 1].v, face[i + 1].n, face[i + 1].t);
			}
        }

    }
}

void CMesh::CreateCube()
{
		AddVertex(glm::vec3(-0.5, -0.5, 0.5));
		AddVertex(glm::vec3(-0.5, 0.5, 0.5));
		AddVertex(glm::vec3(0.5, 0.5, 0.5));
		AddVertex(glm::vec3(0.5, -0.5, 0.5));
		AddVertex(glm::vec3(-0.5, -0.5, -0.5));
		AddVertex(glm::vec3(-0.5, 0.5, -0.5));
		AddVertex(glm::vec3(0.5, 0.5, -0.5));
		AddVertex(glm::vec3(0.5, -0.5, -0.5));

		AddNormal(glm::vec3(1, 0, 0));
		AddNormal(glm::vec3(-1, 0, 0));
		AddNormal(glm::vec3(0, 1, 0));
		AddNormal(glm::vec3(0, -1, 0));
		AddNormal(glm::vec3(0, 0, 1));
		AddNormal(glm::vec3(0, 0, -1));

		AddTexCoord(glm::vec2(0, 0));
		AddTexCoord(glm::vec2(1, 0));
		AddTexCoord(glm::vec2(0, 1));
		AddTexCoord(glm::vec2(1, 1));

		AddTriangleVertex(1, 5, 0);
		AddTriangleVertex(0, 5, 1);
		AddTriangleVertex(3, 5, 2);
		AddTriangleVertex(1, 5, 0);
		AddTriangleVertex(3, 5, 2);
		AddTriangleVertex(2, 5, 3);
		AddTriangleVertex(2, 0, 0);
		AddTriangleVertex(3, 0, 1);
		AddTriangleVertex(7, 0, 2);
		AddTriangleVertex(2, 0, 0);
		AddTriangleVertex(7, 0, 2);
		AddTriangleVertex(6, 0, 3);
		AddTriangleVertex(3, 3, 0);
		AddTriangleVertex(0, 3, 1);
		AddTriangleVertex(4, 3, 2);
		AddTriangleVertex(3, 3, 0);
		AddTriangleVertex(4, 3, 2);
		AddTriangleVertex(7, 3, 3);
		AddTriangleVertex(6, 2, 0);
		AddTriangleVertex(5, 2, 1);
		AddTriangleVertex(1, 2, 2);
		AddTriangleVertex(6, 2, 0);
		AddTriangleVertex(1, 2, 2);
		AddTriangleVertex(2, 2, 3);
		AddTriangleVertex(4, 5, 0);
		AddTriangleVertex(5, 5, 1);
		AddTriangleVertex(6, 5, 2);
		AddTriangleVertex(4, 5, 0);
		AddTriangleVertex(6, 5, 2);
		AddTriangleVertex(7, 5, 3);
		AddTriangleVertex(5, 1, 0);
		AddTriangleVertex(4, 1, 1);
		AddTriangleVertex(0, 1, 2);
		AddTriangleVertex(5, 1, 0);
		AddTriangleVertex(0, 1, 2);
		AddTriangleVertex(1, 1, 3);
}
