// Vita3K emulator project
// Copyright (C) 2025 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

struct FeatureState {
    bool support_shader_interlock = false; ///< First option for blending. Using this with ordered execution mode.
    bool support_texture_barrier = false; ///< Second option for blending. Slower but work on 3 vendors.
    bool direct_fragcolor = false;
    bool spirv_shader = false;
    bool support_get_texture_sub_image = false;
    bool preserve_f16_nan_as_u16 = false; ///< Emit store of 4xU16 to draw buffer 1. This buffer is expected to be U16U16U16U16, which can be casted to F16F16F16F16. This is to preserve some drivers's behaviour of casting NaN to default value when store in framebuffer, not keeping its original value.
    bool support_unknown_format = false;
    bool support_rgb_attributes = true; ///< Do the GPU supports RGB (3 components) vertex attribute? If not (AMD GPU), some modifications must be applied to the renderer and the shader recompiler
    bool use_mask_bit = false; ///< Is the mask bit (1 per sample) emulated ? It is only used in homebrews afaik
    // even though the image being rendered has 8-bit components,
    // the component width while each sample is stored in the tile renderer
    // can be wider and used as a buffer using direct fragcolor access
    // Tearaway does this and the only way to emulate this on an immediate
    // renderer is to use a texture with a big enough component size
    bool use_rgba16_for_rgba8 = false;
    bool support_memory_mapping = false; ///< Is the host GPU memory directly mapped with gxm memory?
    bool use_texture_viewport = false; ///< Are we using texture viewports in the shader

    bool is_programmable_blending_supported() const {
        return support_shader_interlock || support_texture_barrier || direct_fragcolor;
    }

    bool is_programmable_blending_need_to_bind_color_attachment() const {
        return (support_texture_barrier || support_shader_interlock) && !direct_fragcolor;
    }

    bool should_use_shader_interlock() const {
        return support_shader_interlock && !direct_fragcolor;
    }

    bool should_use_texture_barrier() const {
        return support_texture_barrier && !support_shader_interlock && !direct_fragcolor;
    }
};
