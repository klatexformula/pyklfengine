# run from within build directory
import sys
sys.path.insert(0, ".")

import logging
logging.basicConfig()

import pyklfengine

s = pyklfengine.settings.detect_settings()

i = pyklfengine.input(latex=r'z(\alpha) = 1- \int_0^\infty z(t)\,dt')
i.dpi = 180
i.bg_color = pyklfengine.color(0,0,127,40)
i.margins = pyklfengine.margins(5,5,5,5)
i.parameters = {'bg_frame': {'color': '127,0,0', 'x_offset': '2pt'}}

i2 = pyklfengine.input(i)
i2.latex = 'z+z+z=3z'

fmt = pyklfengine.format_spec('PNG')

which_class = pyklfengine.engines.klflatexpackage
#which_class = pyklfengine.engines.latextoimage

engine_instance = which_class.engine()
engine_instance.set_settings(s)

run_instance_1 = engine_instance.run(i)
run_instance_2 = engine_instance.run(i2)

for r in (run_instance_1, run_instance_2):
    r.compile()


for j, r in enumerate([run_instance_1, run_instance_2]):
    d = r.get_data(fmt)
    #print(d)
    with open("simpletest-call-engine-{}.{}".format(j, fmt.format.lower()), "wb") as f:
       f.write(d)
