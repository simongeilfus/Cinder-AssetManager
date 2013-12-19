#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"

#include "AssetManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AsyncSurfaceLoadingApp : public AppNative {
  public:
	void setup();
	void draw();
    
    Surface mSurface;
};

void AsyncSurfaceLoadingApp::setup()
{
    setWindowSize( 512, 512 );
    
    // Enable alpha blending in case our image supports it.
    gl::enableAlphaBlending();
    
    cout << "Main thread id: " << std::this_thread::get_id() << endl;
    
    // By default the asset will automaticaly be watched and reloaded on the fly.
    // Try opening "cinder_logo_alpha.png" in photoshop, make some modifications
    // and press cmd/ctrl-s.
    
    // By modifying the AssetManger::Options passed through the load function you can
    // specify if you want asynchronous loading. In that case the loading will occur in
    // a separated thread, and will not block the main thread. Be carefull what you do
    // inside that thread.
    //
    // ( AssetManager::Options().asynchronous() )
    AssetManager::load( "cinder_logo_alpha.png", [this](DataSourceRef dataSource){
        
        // Thanks to c++11 support on all platform we can use lambdas and
        // there's no need to write an extra function anywhere else.
        mSurface = Surface( loadImage( dataSource ) );
        
        // Because we are in a different thread this might not work:
        // setWindowSize( mSurface.getWidth(), mSurface.getHeight() );
        
        cout << "Loading thread id: " << std::this_thread::get_id() << endl;
        
    }, AssetManager::Options().asynchronous() );
}

void AsyncSurfaceLoadingApp::draw()
{
	gl::clear( ColorA::white() );
    
    if( mSurface ){
        gl::Texture tex( mSurface );
        gl::draw( tex );
    }
}

CINDER_APP_NATIVE( AsyncSurfaceLoadingApp, RendererGl )
