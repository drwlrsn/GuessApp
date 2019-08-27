open ReactNative;

module Colors = {
  let primary = "#f7287b";
  let accent = "#c717fc";
};

module Typography = {
  let style =
    Style.(
      StyleSheet.create({
        "body": style(~fontFamily="open-sans", ()),
        "title": style(~fontFamily="open-sans-bold", ~fontSize=18., ()),
      })
    );
};