#pragma once  // ��ֹͷ�ļ����ظ�����

#include <SFML/graphics.hpp>  // ����SFMLͼ��ģ��
#include <string>             // �����ַ�����
using std::string;            // ʹ�ñ�׼���е�string

// �����࣬�̳���SFML��Sprite��
class Background : public sf::Sprite
{
public:
    // ���캯��
    // ������
    //   texture - ������������
    //   position - ������ʼλ�ã�Ĭ��Ϊ(0,0)
    Background(const sf::Texture& texture, sf::Vector2f position = { 0.f, 0.f });

private:
    sf::Texture texture;  // �洢��������
    float scale;          // �������ű�������Ȼ�����˵�δʹ�ã�
};

// ���캯��ʵ��
Background::Background(const sf::Texture& texture, sf::Vector2f position)
    : sf::Sprite(texture),   // ��ʼ������Sprite
    texture(texture)       // ��ʼ����Աtexture�����������ɲ���Ҫ�Ŀ�����
{
    // ���þ���ʹ�õ�����
    setTexture(this->texture);

    // ���þ���λ��
    setPosition(position);

    // ��ȡ��ǰ����ֱ���
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // ���㲢�������ű�����ʹ��������������Ļ
    // ���㷽������Ļ���/�����ȣ���Ļ�߶�/����߶�
    setScale({
        (float)desktop.size.x / texture.getSize().x,  // ˮƽ���ű���
        (float)desktop.size.y / texture.getSize().y   // ��ֱ���ű���
        });
}