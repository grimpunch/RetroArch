//
//  metal_common.h
//  RetroArch_Metal
//
//  Created by Stuart Carnie on 5/14/18.
//

#ifndef METAL_COMMON_H__
#define METAL_COMMON_H__

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "metal/metal_common.h"

#include <retro_common_api.h>
#include "../drivers_shader/slang_process.h"

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

RETRO_BEGIN_DECLS

extern MTLPixelFormat glslang_format_to_metal(glslang_format fmt);
extern MTLPixelFormat SelectOptimalPixelFormat(MTLPixelFormat fmt);

#pragma mark - Classes

@interface FrameView : NSObject

@property (nonatomic, readonly) RPixelFormat format;
@property (nonatomic, readonly) RTextureFilter filter;
@property (nonatomic, readwrite) BOOL visible;
@property (nonatomic, readwrite) CGRect frame;
@property (nonatomic, readwrite) CGSize size;
@property (nonatomic, readonly) ViewDrawState drawState;
@property (nonatomic, readonly) struct video_shader *shader;
@property (nonatomic, readwrite) uint64_t frameCount;

- (void)setFilteringIndex:(int)index smooth:(bool)smooth;
- (BOOL)setShaderFromPath:(NSString *)path;
- (void)updateFrame:(void const *)src pitch:(NSUInteger)pitch;
- (bool)readViewport:(uint8_t *)buffer isIdle:(bool)isIdle;

@end

@interface MetalMenu : NSObject

@property (nonatomic, readonly) bool hasFrame;
@property (nonatomic, readwrite) bool enabled;
@property (nonatomic, readwrite) float alpha;

- (void)updateFrame:(void const *)source;

- (void)updateWidth:(int)width
             height:(int)height
             format:(RPixelFormat)format
             filter:(RTextureFilter)filter;
@end

@interface Overlay : NSObject
@property (nonatomic, readwrite) bool enabled;
@property (nonatomic, readwrite) bool fullscreen;

- (bool)loadImages:(const struct texture_image *)images count:(NSUInteger)count;
- (void)updateVertexX:(float)x y:(float)y w:(float)w h:(float)h index:(NSUInteger)index;
- (void)updateTextureCoordsX:(float)x y:(float)y w:(float)w h:(float)h index:(NSUInteger)index;
- (void)updateAlpha:(float)alpha index:(NSUInteger)index;
@end

@interface MetalDriver : NSObject<MTKViewDelegate>

@property (nonatomic, readonly) video_viewport_t *viewport;
@property (nonatomic, readwrite) bool keepAspect;
@property (nonatomic, readonly) MetalMenu *menu;
@property (nonatomic, readonly) FrameView *frameView;
@property (nonatomic, readonly) MenuDisplay *display;
@property (nonatomic, readonly) Overlay *overlay;
@property (nonatomic, readonly) Context *context;
@property (nonatomic, readonly) Uniforms *viewportMVP;

- (instancetype)initWithVideo:(const video_info_t *)video
                        input:(const input_driver_t **)input
                    inputData:(void **)inputData;

- (void)setVideo:(const video_info_t *)video;
- (bool)renderFrame:(const void *)data
              width:(unsigned)width
             height:(unsigned)height
         frameCount:(uint64_t)frameCount
              pitch:(unsigned)pitch
                msg:(const char *)msg
               info:(video_frame_info_t *)video_info;

/*! @brief setNeedsResize triggers a display resize */
- (void)setNeedsResize;
- (void)setViewportWidth:(unsigned)width height:(unsigned)height forceFull:(BOOL)forceFull allowRotate:(BOOL)allowRotate;
- (void)setRotation:(unsigned)rotation;

@end

RETRO_END_DECLS

#endif
