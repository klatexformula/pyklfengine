
# run from within build directory
import sys
sys.path.insert(0, ".")
# sys.path.insert(0, "./build")
# sys.path.insert(0, "..")
# sys.path.insert(0, "../build")

import pyklfengine

inp = pyklfengine.input(
    latex=r"Z = \int_0^\infty f(x)\,dx",
    font_size=12,
    preamble="\\usepackage{amsmath}",
    math_mode=(r"$\begin{aligned}", r"\end{aligned}$")
)

#inp.latex_engine = "latex"
inp.latex_engine = "pdflatex"
#inp.latex_engine = "lualatex"


inp.parameters = {
    #"baseline_rule": False
    "baseline_rule": {"type": "line", "setup": r"\color{red!25!black}", "thickness": r"0.25pt"}
}
inp.preamble += r"\usepackage{xcolor}"
# inp.preamble += r"""
# \usepackage{xcolor}
# \usepackage{lua-visual-debug} % debug TeX boxes :)
# """

print(pyklfengine.margins(0.0, 3.0))
print(pyklfengine.margins(*[0.5, 1.5, 2.52, 3.53]))

#inp.fg_color = pyklfengine.color(0,80,80)
inp.margins = pyklfengine.margins(1, 3.5120421, 1, 3.5001)

print("A")
pdf_data = pyklfengine.engines.klflatexpackage.compile_to(inp, "PDF")
with open("tmp-klf-test-output.pdf", "wb") as f:
   f.write(pdf_data)

print("B")
altpdf_data = pyklfengine.engines.latextoimage.compile_to(inp, "PDF")
with open("tmp-klf-test-output-alt.pdf", "wb") as f:
   f.write(altpdf_data)

print("C")
inp2 = inp
inp2.bg_color = pyklfengine.color(255,194,221)
pdf2_data = pyklfengine.engines.latextoimage.compile_to(inp2, "PDF")
with open("tmp-klf-test-output-2.pdf", "wb") as f:
    f.write(pdf2_data)

print("D")
png_data = pyklfengine.engines.latextoimage.compile_to(inp, "PNG")
with open("tmp-klf-test-output.png", "wb") as f:
    f.write(png_data)

print("Done")
