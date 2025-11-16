//
// Created by blueberry on 2025/11/16.
//

#ifndef MYOPENGL_UNIFORMBUFFER_H
#define MYOPENGL_UNIFORMBUFFER_H


class UniformBuffer {
public:
    UniformBuffer(const void *data, unsigned int size);

    ~UniformBuffer();

    void Bind() ;
    void Unbind() ;

    // Bind to point
    void BindRangeToPoint(unsigned int point, unsigned int offset, unsigned int size);

    // Update data.
    void UpdateData(unsigned int offset, unsigned int size, const void *data) ;

private :
    unsigned int m_RendererID{};
};


#endif //MYOPENGL_UNIFORMBUFFER_H
