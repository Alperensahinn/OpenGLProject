#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <PxPhysicsAPI.h>

#include <iostream>
#include <vector>
#include <chrono>

#include "stb_image.h"
#include "OpenGLError.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "DefaultCube.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "GenericDrawableObject.h"
#include "Scene.h"
#include "RigidBody.h"

GLFWwindow* window;

//frame rate
int _fpsCount = 0;
int fps = 0;
void CalculateFrameRate();
std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();

//inputs
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

//physx

//mouse
float xpos;
float ypos;

//window size
const unsigned int W_WIDTH = 1600;
const unsigned int W_HEIGHT = 900;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "INITIALIZING...", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSwapInterval(0);  //disable v-sync

    //glad loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //Some opengl stuff
    //enables openglErrorHandling
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    glfwSetCursorPosCallback(window, mouse_callback);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    //depth test
    glEnable(GL_DEPTH_TEST);
    
    
    //face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
       

    //opengl version check
    std::cout << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    //Some opengl stuff

    //Shader
    Shader shader("src/shader.vert", "src/shader.frag");
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setInt("shadowMap", 2);
    shader.setInt("material.normalMap", 3);

    //projection and view matrix
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(70.0f), (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 100.0f);

    //GameObjects
    Scene* scene = new Scene(window, shader);



    //shadow map (no abstraction for first implementation and testing)
    Shader simpleDepthShader("src/DepthShader.vert", "src/DepthShader.frag");
    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    //shadow call matrices
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = -50.0f, far_plane = 50.0f;

    lightProjection = glm::ortho(50.0f, -50.0f, -50.0f, 50.0f, near_plane, far_plane);
 
    lightView = glm::lookAt(scene->light->attrib.direction, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;






    //PHYSX (no abstraction for first implementation and testing)


    //init
    static physx::PxDefaultAllocator gDefaultAllocatorCallback;
    static physx::PxDefaultErrorCallback gDefaultErrorCallback;
    static physx::PxDefaultCpuDispatcher* mCpuDispatcher;
    static physx::PxPvd* mPvd;

    physx::PxFoundation* mFoundation;
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (!mFoundation)
        std::cout << "PHYSX ERROR!!: PxCreateFoundation failed!" << std::endl;

    bool recordMemoryAllocations = true;

    mPvd = physx::PxCreatePvd(*mFoundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

    physx::PxPhysics* mPhysics;
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, physx::PxTolerancesScale(), recordMemoryAllocations, mPvd);
    if (!mPhysics) 
    {
        std::cout << "PHYSX ERROR!!: PxCreatePhysics failed!" << std::endl;
        return -1;
    }

    physx::PxCooking* mCooking;
    physx::PxTolerancesScale tolerancesScale;
    tolerancesScale.length = 1;
    tolerancesScale.speed = 10;
    mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(tolerancesScale));
    if (!mCooking)
        std::cout << "PHYSX ERROR!!: PxCreateCooking failed!" << std::endl;


    //scene
    physx::PxScene* mScene;
    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = mCpuDispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    mScene = mPhysics->createScene(sceneDesc);
    if (!mScene)
        std::cout << "PHYSX ERROR!!: createScene failed!" << std::endl;


    if (!PxInitExtensions(*mPhysics, mPvd))
        std::cout << "PHYSX ERROR!!: PxInitExtensions failed!" << std::endl;

    physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();

    if(pvdClient)
    {
        pvdClient->setScenePvdFlags(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS | physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS | physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES);
    }



    //material
    physx::PxMaterial* mMaterial;

    mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.05f);    //static friction, dynamic friction, restitution
    if (!mMaterial) 
    {
        std::cout << "PHYSX ERROR!!: createMaterial failed!" << std::endl;
        return -1;
    }


    physx::PxReal denstiy = 10;
    physx::PxRigidDynamic* aCubeActor = physx::PxCreateDynamic(*mPhysics, physx::PxTransform(physx::PxVec3(0.0f, 70.0f, 0.0f)), physx::PxBoxGeometry(1.0f, 1.0f, 1.0f),
        *mMaterial, denstiy);

    if(aCubeActor == nullptr)
    {
        std::cout << "PHYSX ERROR!!: create aCubeActor failed!" << std::endl;
        return -1;
    }
    mScene->addActor(*aCubeActor);


    
    physx::PxRigidDynamic* aCubeActor2 = physx::PxCreateDynamic(*mPhysics, physx::PxTransform(physx::PxVec3(1.0f, 50.0f, 0.0f)), physx::PxBoxGeometry(1.0f, 1.0f, 1.0f),
        *mMaterial, denstiy);
    if (aCubeActor2 == nullptr)
    {
        std::cout << "PHYSX ERROR!!: create aCubeActor failed!" << std::endl;
        return -1;
    }
    mScene->addActor(*aCubeActor2);


    
    physx::PxRigidStatic* plane = PxCreatePlane(*mPhysics, physx::PxPlane(physx::PxVec3(0, 1, 0), 0), *mMaterial);
    if (plane == nullptr)
    {
        std::cout << "FATAL ERROR!!: create shape failed!" << std::endl;
        return -1;
    }
    mScene->addActor(*plane);






    RBDesc rbdesc;
    rbdesc.mPhysics = mPhysics;
    rbdesc.mCooking = mCooking;
    rbdesc.mScene = mScene;

    rbdesc.mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.05f);

    rbdesc.shape = 1;
    rbdesc.type = 0;
    
    GenericDrawableObject* gdo = dynamic_cast<GenericDrawableObject*>(scene->gm[8]);
    gdo->AddRigidBody(rbdesc);



    /*
    physx::PxRigidDynamic* meshActor = mPhysics->createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f,0.0f,0.0f)));
    physx::PxShape* meshShape;

    if (meshActor)
    {
        meshActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

        physx::PxTriangleMeshGeometry triGeom;
        triGeom.triangleMesh = convexMesh;

        meshShape = physx::PxRigidActorExt::createExclusiveShape(*meshActor, triGeom,
            *mMaterial);
    }
    */
    
    /*
    physx::PxRigidDynamic* aConvexActor = mPhysics->createRigidDynamic(physx::PxTransform(physx::PxVec3(1.0f, 0.0f, 0.0f)));
    physx::PxShape* aConvexShape = physx::PxRigidActorExt::createExclusiveShape(*aConvexActor,
        physx::PxTriangleMeshGeometry(convexMesh), *mMaterial);
        */









    
    //PHYSX






    while (!glfwWindowShouldClose(window))
    {
        //time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input handling
        processInput(window);
        scene->camera->ProcessMouseInput(xpos, ypos);
        scene->camera->Update(deltaTime);
        view = glm::lookAt(scene->camera->position, scene->camera->position + scene->camera->front, scene->camera->up);



        //physx test
        mScene->simulate(1.0f/144.0f);
        mScene->fetchResults(true);

        physx::PxVec3 aPos = aCubeActor->getGlobalPose().p;
        physx::PxQuat pxQuat = aCubeActor->getGlobalPose().q;
        
        glm::quat glmQuat = glm::quat(pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z);

        glm::vec3 euler = glm::eulerAngles(glmQuat);

        scene->gm[5]->SetPosition(0, aPos.x, aPos.y, aPos.z);
        scene->gm[5]->SetRotation(0, glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));

        aPos = aCubeActor2->getGlobalPose().p;
        pxQuat = aCubeActor2->getGlobalPose().q;

        glmQuat = glm::quat(pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z);

        euler = glm::eulerAngles(glmQuat);

        scene->gm[5]->SetPosition(1, aPos.x, aPos.y, aPos.z);
        scene->gm[5]->SetRotation(1, glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
        //physx test



        //fbo
        //render scene
        glCullFace(GL_BACK);
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        for (unsigned int i = 0; i < scene->gm.size(); i++)
        {
            if (Drawable* dr = dynamic_cast<Drawable*>(scene->gm[i])) //render cycle
            {
                for(unsigned int j = 0; j < scene->gm[i]->modelInstances.size(); j++)
                {
                    simpleDepthShader.setMat4("model", scene->gm[i]->modelInstances[j].model);   //model location
                    dr->Bind();
                    dr->Draw(); //draw call
                }
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        //render
        glCullFace(GL_FRONT);
        glViewport(0, 0, W_WIDTH, W_HEIGHT);
        glClearColor(0.67f, 0.84f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        scene->DrawScene(shader, projection, view, lightSpaceMatrix, depthMap);

        glfwSwapBuffers(window);
        glfwPollEvents();

        CalculateFrameRate();
    }

    delete scene;

    PxCloseExtensions();
    mPhysics->release();
    mFoundation->release();
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    xpos = static_cast<float>(xposIn);
    ypos = static_cast<float>(yposIn);
}

void CalculateFrameRate() {
    auto currentTime = std::chrono::steady_clock::now();

    const auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
    ++_fpsCount;

    if (elapsedTime > 1000000000) {
        lastTime = currentTime;
        fps = _fpsCount;
        _fpsCount = 0;

        glfwSetWindowTitle(window, std::to_string(fps).c_str());
    }
}




