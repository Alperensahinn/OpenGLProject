#include "Scene.h"

Scene::Scene(GLFWwindow* window, Shader shader)
{
    camera = new Camera(window);

    light =  new Light("light", window);
    shader.setVec3("light.direction", light->attrib.direction);
    shader.setVec3("light.ambient", light->attrib.ambient);
    shader.setVec3("light.diffuse", light->attrib.diffuse);
    shader.setVec3("light.specular", light->attrib.specular);
    shader.setVec3("lightPos", light->attrib.direction);
    
    gm.push_back(new GenericDrawableObject("000", window, "Resources/Plane_20x20m.glb", "Resources/ground/forest_ground_04_diff_2k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/ground/forest_ground_04_nor_gl_2k.jpg"));
    gm[0]->AddInstance(0.0f, 0.0f, 0.0f);

    gm.push_back(new GenericDrawableObject("001", window, "Resources/Walls.glb", "Resources/walls/rough_plaster_03_diff_2k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/walls/rough_plaster_03_nor_gl_2k.jpg"));
    gm[1]->AddInstance(0.0f, 0.0f, 0.0f);

    gm.push_back(new GenericDrawableObject("002", window, "Resources/Platform01/Platform01_Wall.fbx", "Resources/Platform01/textures/castle_brick_02_red_diff_4k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/Platform01/textures/castle_brick_02_red_nor_gl_4k.jpg"));
    gm[2]->AddInstance(0.0f, 0.0f, 0.0f);
    gm[2]->AddInstance(0.0f, 0.0f, 0.0f);
    gm[2]->SetRotation(1, 0.0f, 180.0f, 0.0f);

    gm.push_back(new GenericDrawableObject("003", window, "Resources/Platform01/Platform01_WoodenFloor.fbx", "Resources/Platform01/textures/wood_planks_dirt_diff_4k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/Platform01/textures/wood_planks_dirt_nor_gl_4k.jpg"));
    gm[3]->AddInstance(0.0f, 0.0f, 0.0f);
    gm[3]->AddInstance(0.0f, 0.0f, 0.0f);
    gm[3]->SetRotation(1, 0.0f, 180.0f, 0.0f);

    gm.push_back(new GenericDrawableObject("004", window, "Resources/Platform01/Platform01_Pillars.fbx", "Resources/Platform01/textures/castle_brick_02_red_diff_4k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/Platform01/textures/castle_brick_02_red_nor_gl_4k.jpg"));
    gm[4]->AddInstance(0.0f, 0.0f, 0.0f);
    gm[4]->AddInstance(0.0f, 0.0f, 0.0f);
    gm[4]->SetRotation(1, 0.0f, 180.0f, 0.0f);

    gm.push_back(new DefaultCube("005", window));
    gm[5]->AddInstance(0.0f, 100.0f, 0.0f);
    gm[5]->AddInstance(0.0f, 100.f, 10.0f);

    gm.push_back(new GenericDrawableObject("006", window, "Resources/Platform01/Platform01_Stairs.fbx", "Resources/Platform01/textures/cracked_concrete_wall_diff_2k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/Platform01/textures/cracked_concrete_wall_nor_gl_2k.jpg"));
    gm[6]->AddInstance(0.0f, 0.0f, 0.0f);
    gm[6]->AddInstance(0.0f, 0.0f, 0.0f);
    gm[6]->SetRotation(1, 0.0f, 180.0f, 0.0f);

    gm.push_back(new GenericDrawableObject("007", window, "Resources/wooden_stool/wooden_stool_01.fbx", "Resources/wooden_stool/textures/wooden_stool_01_diff_2k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/wooden_stool/textures/wooden_stool_01_nor_gl_2k.jpg"));
    gm[7]->AddInstance(0.0f, 0.0f, 5.0f);

    gm.push_back(new GenericDrawableObject("008", window, "Resources/wooden_chair/WoodenChair_01.fbx", "Resources/wooden_chair/textures/WoodenChair_01_diff_2k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/wooden_chair/textures/WoodenChair_01_nor_gl_2k.jpg"));
    gm[8]->AddInstance(0.0f, 0.0f, 8.0f);

}

Scene::~Scene()
{
    while (!this->gm.empty()) {
        delete this->gm.back();
        this->gm.pop_back();
    }

    std::cout << "gm contains: " << !this->gm.empty() << std::endl;

    delete camera;
    delete light;
}

void Scene::DrawScene
(Shader shader, glm::mat4 projection, glm::mat4 view, glm::mat4 lightSpaceMatrix, unsigned int depthMap)
{
    //lights
    shader.setVec3("lightPos", this->light->attrib.direction);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    //material uniforms
    shader.setFloat("material.shininess", 32.0f);

    //camera position
    shader.setVec3("viewPos", camera->position);

    for (unsigned int i = 0; i < this->gm.size(); i++)
    {
        if (this->gm[i]->update == true)
        {
            this->gm[i]->Update();
        }

        if (this->gm[i]->fixedUpdate == true)
        {
            this->gm[i]->FixedUpdate();
        }
        //NewType* v = dynamic_cast<NewType*>(old)
        if (Drawable* dr = dynamic_cast<Drawable*>(this->gm[i])) //render cycle
        {
            //dr->Bind();

            for (unsigned int j = 0; j < gm[i]->modelInstances.size(); j++) 
            {
                shader.setMat4("model", this->gm[i]->modelInstances[j].model);   //model location
                shader.setMat4("normal_model", glm::transpose(glm::inverse(this->gm[i]->modelInstances[j].model))); //normal vectors
                shader.setMat4("PVM", projection * view * this->gm[i]->modelInstances[j].model); //PVM

                dr->Bind();
                dr->Draw(); //draw call
            }
        }
    }
}
