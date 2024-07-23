ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := tetris

all: bitmaps tilemaps $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx
	
#---------------------------------------------------------------------------------
pvsneslibfont.pic: pvsneslibfont.png
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -e 0 -i $<

#---------------------------------------------------------------------------------
minoset1.pic: minoset1.png
	@echo convert minoset1 ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -t png -i $<
minoset2.pic: minoset2.png
	@echo convert minoset2 ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -t png -i $<

boardedges.pic: boardedges.png
	@echo convert boardedges ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -t png -i $<

#---------------------------------------------------------------------------------
tilemap.bin: mockup.json
	@echo convert tilemap ... $(notdir $@)
	python3 convert_mockup_tilemap.py

bitmaps : pvsneslibfont.pic minoset1.pic minoset2.pic boardedges.pic
tilemaps : tilemap.bin
