class CanvImpl;
class ImageImpl;

class Image {
public:
    ImageImpl* GetImpl() {
        return im_ipml;
    }
private:
    ImageImpl* im_ipml;
};

class Canv {
public:
    void Draw(Image image);
private:
    CanvImpl* canv_impl;
};