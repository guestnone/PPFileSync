set(PPFS_INC_NOFILTER
	"PPFSPrerequisites.h"
)

set(PPFS_SRC_NOFILTER
	"PPFSMain.c"
)

source_group("" FILES ${PPFS_INC_NOFILTER} ${PPFS_SRC_NOFILTER})

set(PPFS_SRC
	${PPFS_INC_NOFILTER}
	${PPFS_SRC_NOFILTER}
)