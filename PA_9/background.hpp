#pragma once  // 防止头文件被重复包含

#include <SFML/graphics.hpp>  // 包含SFML图形模块
#include <string>             // 包含字符串库
using std::string;            // 使用标准库中的string

// 背景类，继承自SFML的Sprite类
class Background : public sf::Sprite
{
public:
    // 构造函数
    // 参数：
    //   texture - 背景纹理引用
    //   position - 背景初始位置，默认为(0,0)
    Background(const sf::Texture& texture, sf::Vector2f position = { 0.f, 0.f });

private:
    sf::Texture texture;  // 存储背景纹理
    float scale;          // 背景缩放比例（虽然声明了但未使用）
};

// 构造函数实现
Background::Background(const sf::Texture& texture, sf::Vector2f position)
    : sf::Sprite(texture),   // 初始化基类Sprite
    texture(texture)       // 初始化成员texture（这里可能造成不必要的拷贝）
{
    // 设置精灵使用的纹理
    setTexture(this->texture);

    // 设置精灵位置
    setPosition(position);

    // 获取当前桌面分辨率
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // 计算并设置缩放比例，使背景填满整个屏幕
    // 计算方法：屏幕宽度/纹理宽度，屏幕高度/纹理高度
    setScale({
        (float)desktop.size.x / texture.getSize().x,  // 水平缩放比例
        (float)desktop.size.y / texture.getSize().y   // 垂直缩放比例
        });
}