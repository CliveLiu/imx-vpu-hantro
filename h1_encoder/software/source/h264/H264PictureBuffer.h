/*------------------------------------------------------------------------------
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                                                                            --
--                            Hantro Products Oy.                             --
--                                                                            --
--                   (C) COPYRIGHT 2006 HANTRO PRODUCTS OY                    --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
------------------------------------------------------------------------------*/

#ifndef H264PICTURE_BUFFER_H
#define H264PICTURE_BUFFER_H

/*------------------------------------------------------------------------------
	Include headers
------------------------------------------------------------------------------*/
#include "basetype.h"
#include "encasiccontroller.h"

/*------------------------------------------------------------------------------
	External compiler flags
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
	Module defines
------------------------------------------------------------------------------*/
#define BUFFER_SIZE	4   /* to support svct */

typedef struct {
	i32 lumWidth;		/* Width of *lum */
	i32 lumHeight;		/* Height of *lum */
	i32 chWidth;		/* Width of *cb and *cr */
	i32 chHeight;		/* Height of *cb and *cr */
	ptr_t lum;
	ptr_t cb;
} picture;

typedef struct refPic {
	picture picture;	/* Image data */
	i32 poc;		/* Picture order count */
	i32 frameNum;

	bool i_frame;		/* I frame (key frame), only intra mb */
	bool p_frame;		/* P frame, intra and inter mb */
	bool show;		/* Frame is for display (showFrame flag) */
	bool ipf;		/* Frame is immediately previous frame */
	bool arf;		/* Frame is altref frame */
	bool grf;		/* Frame is golden frame */
	bool search;		/* Frame is used for motion estimation */
	struct refPic *refPic;	/* Back reference pointer to itself */
	i32  orderCmd;
} refPic;

typedef struct {
	i32 size;		/* Amount of allocated reference pictures */
	picture input;		/* Input picture */
	refPic refPic[BUFFER_SIZE + 1];	/* Reference picture store */
	refPic refPicList[BUFFER_SIZE];	/* Reference picture list */
	refPic *cur_pic;	/* Pointer to picture under reconstruction */
	refPic *last_pic;	/* Last picture */
} picBuffer;

/*------------------------------------------------------------------------------
	Function prototypes
------------------------------------------------------------------------------*/
i32 H264PictureBufferAlloc(picBuffer *picBuffer, i32 width, i32 height);
void H264PictureBufferFree(picBuffer *picBuffer);
void H264InitializePictureBuffer(picBuffer *picBuffer);
void H264UpdatePictureBuffer(picBuffer *picBuffer);
void H264PictureBufferSetRef(picBuffer *picBuffer, asicData_s *asic, u32 numViews);
i32 H264PictureBufferSetupH264(picBuffer *picBuffer, asicData_s *asic, u32 numRefBuffsLum, u32 numRefBuffsChr);
void H264UpdatePictureBufferForSvct(picBuffer *picBuffer, u32 frameNumBits, i32 gopIndex);
void H264PictureBufferSetRefForSvct(picBuffer *picBuffer, asicData_s *asic, u32 frameNumBits);

#endif
