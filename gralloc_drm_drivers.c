/*
 * Copyright (C) 2010-2011 Chia-I Wu <olvaffe@gmail.com>
 * Copyright (C) 2010-2011 LunarG Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#define LOG_TAG "GRALLOC-DRM"

#include <fcntl.h>

#include <cutils/log.h>

#include "gralloc_drm.h"
#include "gralloc_drm_priv.h"

#define GRALLOC_DRM_DEVICE "/dev/dri/card0"

/*
 * Create the driver for a DRM fd.
 */
static struct gralloc_drm_drv_t *
init_drv_from_fd(int fd)
{
	struct gralloc_drm_drv_t *drv = NULL;
	drmVersionPtr version;

	/* get the kernel module name */
	version = drmGetVersion(fd);
	if (!version) {
		ALOGE("invalid DRM fd");
		return NULL;
	}

	if (version->name) {
#ifdef ENABLE_FREEDRENO
		if (!strcmp(version->name, "msm")) {
			drv = gralloc_drm_drv_create_for_freedreno(fd);
			ALOGI_IF(drv, "create freedreno for driver msm");
		} else
#endif
#ifdef ENABLE_INTEL
		if (!strcmp(version->name, "i915")) {
			drv = gralloc_drm_drv_create_for_intel(fd);
			ALOGI_IF(drv, "create intel for driver i915");
		} else
#endif
#ifdef ENABLE_RADEON
		if (!strcmp(version->name, "radeon")) {
			drv = gralloc_drm_drv_create_for_radeon(fd);
			ALOGI_IF(drv, "create radeon for driver radeon");
		} else
#endif
#ifdef ENABLE_NOUVEAU
		if (!strcmp(version->name, "nouveau")) {
			drv = gralloc_drm_drv_create_for_nouveau(fd);
			ALOGI_IF(drv, "create nouveau for driver nouveau");
		} else
#endif
#ifdef ENABLE_PIPE
		{
			drv = gralloc_drm_drv_create_for_pipe(fd, version->name);
			ALOGI_IF(drv, "create pipe for driver %s", version->name);
		}
#endif
		if (!drv) {
			ALOGE("unsupported driver: %s", (version->name) ?
					version->name : "NULL");
		}
	}

	drmFreeVersion(version);

	return drv;
}

/*
 * Create a DRM device object.
 */
struct gralloc_drm_t *gralloc_drm_create(void)
{
	struct gralloc_drm_t *drm;
	int err;

	drm = calloc(1, sizeof(*drm));
	if (!drm)
		return NULL;

	drm->fd = open(GRALLOC_DRM_DEVICE, O_RDWR);
	if (drm->fd < 0) {
		ALOGE("failed to open %s", GRALLOC_DRM_DEVICE);
		return NULL;
	}

	drm->drv = init_drv_from_fd(drm->fd);
	if (!drm->drv) {
		close(drm->fd);
		free(drm);
		return NULL;
	}

	return drm;
}
