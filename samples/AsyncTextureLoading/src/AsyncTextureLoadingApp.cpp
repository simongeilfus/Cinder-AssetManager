#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "AssetManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AsyncTextureLoadingApp : public AppNative {
public:
	void setup();
	void draw();
    
    gl::TextureRef mTexture;
};

void AsyncTextureLoadingApp::setup()
{
    // Enable alpha blending in case our image supports it.
    gl::enableAlphaBlending();
    
    
    // Load the image in a separated thread and returns the ImageSourceRef
    auto asyncLoad = [](DataSourceRef dataSource){
        ImageSourceRef imageSource = loadImage( dataSource );
        return imageSource;
    };
    
    // The second callback is executed in the main thread so any OpenGL resources can be created here.
    auto textureCreation = [this](ImageSourceRef imageSource){
        mTexture = gl::Texture::create( imageSource );
        
        // It's ok to do that in the main thread:
        setWindowSize( mTexture->getWidth(), mTexture->getHeight() );
    };

    // Use the templated version if you want to pass an object from the loading thread to the main thread. Because we are providing the load function with two callbacks there's no need to specify Options().asynchronous() like we would do with only one callback.
    AssetManager::load<ImageSourceRef>( "cinder_logo_alpha.png", asyncLoad, textureCreation );
    
        
    // The following does exactly the same but is shorter.
    /*
     
    AssetManager::load<ImageSourceRef>( "cinder_logo_alpha.png",
     
     // Load the image in a separated thread and returns the ImageSourceRef
    [this](DataSourceRef dataSource){
        ImageSourceRef imageSource = loadImage( dataSource );
        return imageSource;
    },
     
     // The second callback is executed in the main thread so any OpenGL resources can be created here.
    [this](ImageSourceRef imageSource){
        mTexture = gl::Texture::create( imageSource );
    } );
     
    */
}

void AsyncTextureLoadingApp::draw()
{
	gl::clear( ColorA::white() );
    
    if( mTexture ){
        gl::draw( mTexture );
    }
}

CINDER_APP_NATIVE( AsyncTextureLoadingApp, RendererGl )

