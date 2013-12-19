#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

#include "AssetManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class ShaderLoadingApp : public AppNative {
  public:
	void setup();
	void draw();
    
    gl::GlslProgRef mShader;
};

void ShaderLoadingApp::setup()
{
    
    // Enable read&write to the depth buffer so we can draw our sphere correctly.
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    AssetManager::load( "test.vert", "test.frag", [this]( DataSourceRef vert, DataSourceRef frag ) {
        
        // Load our shader and test if it is correctly compiled
        try {
            mShader = gl::GlslProg::create( vert, frag );
        }
        catch( gl::GlslProgCompileExc exc ){
            console() << exc.what() << endl;
        }
    } );
    
    /*
    // Thanks to initializer lists and variadic templates we can group any number of assets
    // and have the same callback function called for initialization. Works the same way as the
    // non-variadic method with the only difference in the syntax that you can use {} to declare
    // your list of asset paths to load.
    
    // The only downside right now is that lambdas with a capture list can't be implicitly cast to
    // the variadic function pointer AssetManager is using, so the code looks a bit redundant,
    // because you need to specify twice the method signature ( cast + lambda parameters ).
    AssetManager::load( { "test.vert", "test.frag" }, (function<void(DataSourceRef,DataSourceRef)>) [this]( DataSourceRef vert, DataSourceRef frag ) {
        
        // Load our shader and test if it is correctly compiled
        try {
            mShader = gl::GlslProg::create( vert, frag );
        }
        catch( gl::GlslProgCompileExc exc ){
            console() << exc.what() << endl;
        }
    } );*/
    
    
    
    /*
    // Completly useless but same example with a separated thread. A tuple is used to transfer the 
    // two DataSourceRef between the loading thread and the main thread. A std::pair could be use here 
    // instead but the tuple allows more than 2 elements to be grouped.
    AssetManager::load<tuple<DataSourceRef,DataSourceRef>>( { "test.vert", "test.frag" },
                                                           (function<tuple<DataSourceRef,DataSourceRef>(DataSourceRef,DataSourceRef)>) [this]( DataSourceRef vert, DataSourceRef frag ) {
                                                               return make_tuple(vert, frag);
                                                           },
                                                           [this]( tuple<DataSourceRef,DataSourceRef> shader ){
                                                               try {
                                                                   mShader = gl::GlslProg::create( std::get<0>( shader ), std::get<1>( shader ) );
                                                               }
                                                               catch( gl::GlslProgCompileExc exc ){
                                                                   console() << exc.what() << endl;
                                                               }
                                                           });*/
}

void ShaderLoadingApp::draw()
{
    // Clear the window
	gl::clear( Color( 0, 0, 0 ) );
    
    // Set a perspective projection matrix and center
    // the drawing at the center of the window
    gl::setMatricesWindowPersp( getWindowSize() );
    gl::translate( getWindowCenter() );
    
    // Draw a sphere and use our shader if available.
    // Wrapping the shader code into a "if" block ensure
    // that it won't be used if the shader hasn't been validated.
    if( mShader ) mShader->bind();
    gl::drawSphere( Vec3f::zero(), 180, 36 );
    if( mShader ) mShader->unbind();
}

CINDER_APP_NATIVE( ShaderLoadingApp, RendererGl )
