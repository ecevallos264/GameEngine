#ifndef GAMEENGINE_FRAMEBUFFER_H
#define GAMEENGINE_FRAMEBUFFER_H

#include <glad/glad.h>

class Framebuffer {
public:
    Framebuffer() = default;
    ~Framebuffer() { destroy(); }

    bool initialize(int width, int height) {
        if (width <= 0 || height <= 0) return false;

        this->width = width;
        this->height = height;

        // Create framebuffer
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Create color texture attachment
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

        // Create depth/stencil renderbuffer attachment
        glGenRenderbuffers(1, &depthRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRbo);

        // Check framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            destroy();
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        initialized = true;
        return true;
    }

    void destroy() {
        if (fbo) {
            glDeleteFramebuffers(1, &fbo);
            fbo = 0;
        }
        if (colorTexture) {
            glDeleteTextures(1, &colorTexture);
            colorTexture = 0;
        }
        if (depthRbo) {
            glDeleteRenderbuffers(1, &depthRbo);
            depthRbo = 0;
        }
        initialized = false;
    }

    void resize(int newWidth, int newHeight) {
        if (newWidth <= 0 || newHeight <= 0) return;
        if (newWidth == width && newHeight == height) return;

        destroy();
        initialize(newWidth, newHeight);
    }

    void bind() {
        if (!initialized) return;
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint getColorTexture() const { return colorTexture; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isInitialized() const { return initialized; }

private:
    GLuint fbo = 0;
    GLuint colorTexture = 0;
    GLuint depthRbo = 0;
    int width = 0;
    int height = 0;
    bool initialized = false;
};

#endif //GAMEENGINE_FRAMEBUFFER_H
