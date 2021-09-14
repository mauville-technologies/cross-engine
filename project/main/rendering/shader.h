//
// Created by ozzadar on 2021-09-13.
//

#pragma once

#include <cstdint>
#include <string>

namespace OZZ {
    class Shader {
    public:
        Shader(const std::string& vertexCode, const std::string& fragmentCode);
        Shader(const char* vertexPath, const char* fragmentPath);
        void Bind();

        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;

        [[nodiscard]] uint32_t GetID() const { return _id; };

    private:
        void compileShader(const std::string& vertexCode, const std::string& fragmentCode);

    private:
        uint32_t _id = 0;

    };
}


