text1 = "WINDOWS1O";
text2 = "GNU/LINUX";

intersection() {
  $fn = 100;
  text_font = "Liberation Mono:style=Bold";
  rotate([0, 0, 0]) translate([0, 0, -10]) linear_extrude(20)
    text(text1, font = text_font, valign = "center", halign = "center");
  rotate([90, 0, 0]) translate([0, 0, -10]) linear_extrude(20)
    text(text2,  font = text_font, valign = "center", halign = "center");
}