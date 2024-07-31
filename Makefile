ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := tetris

all: bitmaps generators $(ROMNAME).sfc

clean: cleanBuildResOverride cleanRom cleanGfxOverride cleanGeneratedData
	
#---------------------------------------------------------------------------------
pvsneslibfont.pic: res/pvsneslibfont.png
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -e 0 -i $<

#---------------------------------------------------------------------------------
minoset1.pic: res/minoset1.png
	@echo convert minoset1 ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -t png -i $<
minoset2.pic: res/minoset2.png
	@echo convert minoset2 ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -t png -i $<

boardedges.pic: res/boardedges.png
	@echo convert boardedges ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -t png -i $<

#---------------------------------------------------------------------------------
jsonTilemaps: emptyboard.bin emptyboard_edgeless.bin

emptyboard.bin: data/emptyboard.json
	@echo convert emptyboard json tilemap ... $(notdir $@)
	python3 generators/convert_json_tilemap.py $<

emptyboard_edgeless.bin: data/emptyboard_edgeless.json
	@echo convert emptyboard_edgeless json tilemap ... $(notdir $@)
	python3 generators/convert_json_tilemap.py $<

outlineTable: generators/generate_outline_table.py
	@echo generate outline table ...
	python3 $<

tetrominoTable: generators/generate_tetromino_table.py
	@echo generate tetromino table ...
	python3 $<

bitmaps : pvsneslibfont.pic minoset1.pic minoset2.pic boardedges.pic
generators : jsonTilemaps outlineTable tetrominoTable

cleanGeneratedData:
	@echo clean generated data
	@rm -f data/generated/*

cleanBuildResOverride: cleanBuildRes cleanStraySymbols

cleanStraySymbols:
	@echo clean stray debugging symbols
	@rm -f *.sym

cleanGfxOverride:
	@echo clean graphics data \(override\)
	@rm -f **/*.pic **/*.map **/*.pal **/*.pc7 **/*.mp7 **/*.til **/*.m16 **/*.b16 **/*.o16 **/*.t16
