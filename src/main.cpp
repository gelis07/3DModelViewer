#define GLEW_STATIC
#include "pch.h"
#include "Camera.h"
#include "GlAbst/Shader.h"
#include "GlAbst/Texture.h"
#include "GlAbst/Framebuffer.h"
#include "GlAbst/VertexPackage.h"
#include "Model.h"
#include "Light.h"
#include "Material.h"
#include <string>
#include "CubeVertInd.h"
#include "Scenes.h"
#include "Editor/Editor.h"
bool showError = true;
void errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    if(showError) std::cerr << "GL CALLBACK: " << message << " (ID: " << id << ")" << std::endl;
}
void Render(Shader& program, Camera& cam, std::vector<Light>& lights, std::vector<PointLight>& PointLights,
Model& model, Model& cube, const glm::mat4& projection, const Texture& DepthMap, const glm::mat4& lightSpaceMatrix){
    program.Bind();
    program.SetUniform("viewPos", cam.GetPos());
    program.SetUniform("LightCount", (int)(lights.size()));
    program.SetUniform("PointLightCount", (int)(PointLights.size()));
    program.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
    DepthMap.UseSlot(10);
    program.SetUniform("shadowMap", (int)(10));
    for (int i = 0; i < lights.size(); i++)
    {
        lights[i].SetUniforms("lights["+std::to_string(i)+"]", &program);
    }
    for (int i = 0; i < PointLights.size(); i++)
    {
        PointLights[i].SetUniforms("Pointlights["+std::to_string(i)+"]", &program);
    }
    model.Draw(&program);
    cube.Draw(&program);
    program.Unbind();
}
int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        return 1;
    }

    // Create a window and OpenGL context using GLFW
    const unsigned int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D model viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Initialize OpenGL loader using gl3w
    if (glewInit() != 0)
    {
        glfwTerminate();
        return 1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Initialize the GLFW and OpenGL3 ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorCallback, 0);
    Shader program("ShadowMapping.vs", "ShadowMapping.fs");
    Shader DepthProgram("DepthMap.vs", "DepthMap.fs");
    Shader GizmosProgram("Gizmos.vs", "Gizmos.fs");
    Shader PointDepthProgram("PointSP.vs", "PointSP.gs", "PointSP.fs");
    Model model("Backpack/backpack.obj");
    Model model1("Backpack/untitled.obj");
    model.Position = glm::vec3(0);
    model.Scale = glm::vec3(1.0f);
    model1.Position = glm::vec3(0, -10, 0);
    model1.Scale = glm::vec3(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 1000.0f);
    bool done = false;
    float DeltaTime = 0.0f;
    double LastTime = 0.0f;

    Light light1(glm::vec3(12.762f, -13.365f, -3.375f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f));
    Light light2(glm::vec3(-12.762f, -13.365f, 3.375f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f));
    PointLight pLight1(glm::vec3(12.762f, 7.635f, -3.375f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
    PointLight pLight2(glm::vec3(0), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
    Scene scene(SCREEN_WIDTH, SCREEN_HEIGHT, SHADOW_WIDTH, SHADOW_HEIGHT);
    scene.AddPointLight(std::move(pLight1));
    scene.AddPointLight(std::move(pLight2));
    scene.AddDirLight(std::move(light1));
    scene.AddDirLight(std::move(light2));

    scene.AddModel(std::move(model));
    scene.AddModel(std::move(model1));

    // Shader PointLightShadows();
    Texture PLShadow(GL_TEXTURE_CUBE_MAP);
    PLShadow.Bind();
    for (size_t i = 0; i < 6; i++)
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);  

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    GLuint PLFBO;
    glGenFramebuffers(1, &PLFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, PLFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, PLShadow.GetID(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Plane), Plane, GL_STATIC_DRAW);

    Texture lightIcon(GL_TEXTURE_2D, "Assets/idea.png");

    // Pos attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3*sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Camera PointShadowCam;
    float aspect = (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT;
    float near = 1.0f;
    float far = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far); 
    std::vector<glm::mat4> shadowTransforms;
    // showError = false;
    while (!done)
    {
        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DeltaTime = glfwGetTime() - LastTime;
        LastTime = glfwGetTime();


        scene.camera.Input(DeltaTime);
        scene.CalculatePointDepthMap(PointDepthProgram);
        scene.CaclulateDepthMap(DepthProgram);
        scene.Render(program, SCREEN_WIDTH, SCREEN_HEIGHT);

        shadowTransforms.clear();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Render the ImGui frame
        Hierachy(&scene, VAO, lightIcon, GizmosProgram, projection);
        Editor(&scene);
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the front and back buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Check if the window should close
        if (glfwWindowShouldClose(window))
        {
            done = true;
        }

    }

    // Shutdown the ImGui and GLFW backends
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    // Shutdown ImGui
    ImGui::DestroyContext();
    // Shutdown GLFW
    glfwTerminate();

    return 0;
}
