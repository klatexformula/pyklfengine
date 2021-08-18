# run from within build directory
import sys
sys.path.insert(0, ".")

import pyklfengine

s = pyklfengine.settings.detect_settings()
s.gs_method = 'process'

i = pyklfengine.input(latex=r'z(\alpha) = 1- \int_0^\infty z(t)\,dt')
i.bg_color = pyklfengine.color(0,0,127,40)
i.margins = pyklfengine.margins(5,5,5,5)
i.parameters = {'bg_frame': {'color': '127,0,0', 'x_offset': '2pt'}}

fmt = pyklfengine.format_spec(
   'TIFF',
   {
      'dpi': 120,
      #'antialiasing': False
      'antialiasing': {'text_alpha_bits': 4, # equation/text antialised
                       'graphics_alpha_bits': 1,} # red border not antialiased
   }
)
d = pyklfengine.klfimplpkg_engine.compile_to(i, fmt, s)
with open("tmp-klf-b2-out."+fmt.format.lower(), "wb") as f:
   f.write(d)

fmt = pyklfengine.format_spec('PDF', {'latex_raw': True})
d = pyklfengine.klfimplpkg_engine.compile_to(i, fmt, s)
with open("tmp-klf-b2-out-RAW."+fmt.format.lower(), "wb") as f:
   f.write(d)

fmt = pyklfengine.format_spec('EPS', {})
d = pyklfengine.klfimplpkg_engine.compile_to(i, fmt, s)
with open("tmp-klf-b2-out."+fmt.format.lower(), "wb") as f:
   f.write(d)


