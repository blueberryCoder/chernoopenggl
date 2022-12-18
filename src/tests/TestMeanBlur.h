//
// Created by blueberry on 2022/12/14.
//

#pragma once

#include "Test.h"
#include "../Shader.h"
#include "../Texture.h"

//https://jishuin.proginn.com/p/763bfbd6cf4c

namespace test {
    class TestMeanBlur : public Test {

        enum BlurType {
            MEAN,
            GAUSSIAN
        };

    public:

        TestMeanBlur();

        ~TestMeanBlur();

        void OnImGuiRender() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

    private:
        unsigned int m_FBO;
        unsigned int m_ColorTexture;
        unsigned int m_DepthTexture;

        unsigned int m_dstVBO;
        unsigned int m_dstVAO;
        unsigned int m_dstIBO;

        BlurType m_BlurType = GAUSSIAN;

        int fbo_width = 960, fbo_height = 540;

        std::unique_ptr<Texture> m_OrgImgTexture;
        std::unique_ptr<Shader> fboShader;

        std::unique_ptr<Shader> dstShader;

        void CreateFBOColorTexture();
        void CreateDepthTexture();
        void CreateFrameBuffer();

        void DrawOnFBO();

        void CreateDstProgram();
    };

}
