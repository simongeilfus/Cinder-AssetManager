#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "AssetManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class AssetWatchingApp : public AppNative {
public:
	void setup();
	void draw();
    
    gl::TextureRef mTexture;
};

void AssetWatchingApp::setup()
{
    // Enable alpha blending in case our image supports it.
    gl::enableAlphaBlending();
    
    // By default the asset will automaticaly be watched and reloaded on the fly.
    // Try opening "cinder_logo_alpha.png" in photoshop, make some modifications
    // and press cmd/ctrl-s.
    AssetManager::load( "cinder_logo_alpha.png", [this](DataSourceRef dataSource){
        
        // Thanks to c++11 support on all platform we can use lambdas and
        // there's no need to write an extra function anywhere else.
        mTexture = gl::Texture::create( loadImage( dataSource ) );
        
        setWindowSize( mTexture->getWidth(), mTexture->getHeight() );
    } );
}

void AssetWatchingApp::draw()
{
	gl::clear( ColorA::white() );
    
    if( mTexture )
        gl::draw( mTexture );
}

CINDER_APP_NATIVE( AssetWatchingApp, RendererGl )
