open ReactNative;

let makeOptionalStyle = (s, o) =>
  switch (s) {
  | Some(s) => StyleSheet.flatten([|o, s|])
  | None => o
  };

module TitleText = {
  let defaultStyle =
    Style.(style(~fontFamily="open-sans-bold", ~fontSize=18., ()));
  [@react.component]
  let make = (~style=?, ~children) =>
    <Text style={makeOptionalStyle(style, defaultStyle)}> children </Text>;
};

module BodyText = {
  let defaultStyle = Style.(style(~fontFamily="open-sans", ()));
  [@react.component]
  let make = (~style=?, ~children) =>
    <Text style={makeOptionalStyle(style, defaultStyle)}> children </Text>;
};

module Card = {
  let styles =
    Style.(
      StyleSheet.create({
        "default":
          style(
            ~shadowColor="black",
            ~shadowOffset=offset(~width=0., ~height=2.),
            ~shadowRadius=6.,
            ~shadowOpacity=0.26,
            ~elevation=8.,
            ~backgroundColor="white",
            ~padding=20.->dp,
            ~borderRadius=10.,
            (),
          ),
      })
    );

  [@react.component]
  let make = (~style=?, ~children) => {
    <View style={style->makeOptionalStyle(styles##default)}> children </View>;
  };
};

module Header = {
  let styles =
    Style.(
      StyleSheet.create({
        "default":
          style(
            ~width=100.->pct,
            ~height=90.->dp,
            ~paddingTop=36.->dp,
            ~backgroundColor=Constants.Colors.primary,
            ~alignItems=`center,
            ~justifyContent=`center,
            (),
          ),
        "title":
          style(
            ~color="black",
            ~fontSize=18.,
            ~fontFamily="open-sans-bold",
            (),
          ),
      })
    );

  [@react.component]
  let make = (~title) =>
    <View style=styles##default>
      <TitleText style=styles##title> title->React.string </TitleText>
    </View>;
};

module Input = {
  let styles =
    Style.(
      StyleSheet.create({
        "default":
          style(
            ~height=30.->dp,
            ~borderBottomColor="grey",
            ~borderBottomWidth=1.,
            ~marginVertical=10.->dp,
            (),
          ),
      })
    );

  [@react.component]
  let make =
      (
        ~style=?,
        ~blurOnSubmit=false,
        ~autoCapitalize=`none,
        ~autoCorrect=true,
        ~keyboardType=`default,
        ~maxLength=999999999,
        ~onChangeText=_ => (),
        ~value="",
      ) => {
    <TextInput
      style={style->makeOptionalStyle(styles##default)}
      blurOnSubmit
      autoCapitalize
      autoCorrect
      keyboardType
      maxLength
      onChangeText
      value
    />;
  };
};

module NumberContainer = {
  let styles =
    Style.(
      StyleSheet.create({
        "container":
          style(
            ~borderWidth=2.,
            ~borderColor=Constants.Colors.accent,
            ~padding=10.->dp,
            ~borderRadius=10.,
            ~marginVertical=10.->dp,
            ~alignItems=`center,
            ~justifyContent=`center,
            (),
          ),
        "number": style(~color=Constants.Colors.accent, ~fontSize=22., ()),
      })
    );

  [@react.component]
  let make = (~children) =>
    <View style=styles##container>
      <BodyText style=styles##number> children </BodyText>
    </View>;
};

module MainButton = {
  let styles =
    Style.(
      StyleSheet.create({
        "button":
          style(
            ~backgroundColor=Constants.Colors.primary,
            ~paddingVertical=12.->dp,
            ~paddingHorizontal=30.->dp,
            ~borderRadius=25.,
            (),
          ),
        "buttonText":
          style(~color="white", ~fontFamily="open-sans", ~fontSize=18., ()),
      })
    );

  [@react.component]
  let make = (~children, ~onPress) =>
    <TouchableOpacity activeOpacity=0.6 onPress>
      <View style=styles##button>
        <Text style=styles##buttonText> children </Text>
      </View>
    </TouchableOpacity>;
};