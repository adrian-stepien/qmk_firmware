# Animation variant selection system
# Usage: BONGO_VARIANT=badger qmk compile -kb kbdfans/odin75 -km default
# Usage: BONGO_VARIANT=cat qmk compile -kb kbdfans/odin75 -km default  (or omit for default)

# Default to original bongocat if no variant specified
BONGO_VARIANT ?= cat

# Include the bongo animation engine and set appropriate variant flag
ifeq ($(strip $(BONGO_VARIANT)), badger)
    SRC += lib/bongo.c
    OPT_DEFS += -DBONGO_ENABLE -DBONGO_VARIANT_BADGER
else ifeq ($(strip $(BONGO_VARIANT)), cat)
    SRC += lib/bongo.c
    OPT_DEFS += -DBONGO_ENABLE -DBONGO_VARIANT_CAT
else ifneq ($(strip $(BONGO_VARIANT)), none)
    # If unknown variant, default to cat
    SRC += lib/bongo.c
    OPT_DEFS += -DBONGO_ENABLE -DBONGO_VARIANT_CAT
endif
