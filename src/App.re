/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * Converted from https://github.com/facebook/react-native/blob/724fe11472cb874ce89657b2c3e7842feff04205/template/App.js
 * With a few tweaks
 */
open ReactNative;

type reactNativeNewAppScreenColors = {
  .
  "primary": string,
  "white": string,
  "lighter": string,
  "light": string,
  "black": string,
  "dark": string,
};

let fetchFonts = () => {
  Expo.Font.loadAsync([
    ("open-sans", Packager.require("../assets/fonts/OpenSans-Regular.ttf")),
    (
      "open-sans-bold",
      Packager.require("../assets/fonts/OpenSans-Bold.ttf"),
    ),
  ]);
};

/*
 Here is StyleSheet that is using Style module to define styles for your components
 The main different with JavaScript components you may encounter in React Native
 is the fact that they **must** be defined before being referenced
 (so before actual component definitions)
 More at https://reasonml-community.github.io/reason-react-native/en/docs/apis/Style/
 */
[@react.component]
let app = () => {
  let (userNumber, setUserNumber) = React.useState(() => None);
  let (guessRounds, setGuessRounds) = React.useState(() => 0);
  let (appLoading, setAppLoading) = React.useState(() => true);

  let startGameHandler = (selectedNumber: option(int)) => {
    setUserNumber(_ => selectedNumber);
    setGuessRounds(_ => 0);
  };

  let gameOverHandler = (numOfRounds: int) => {
    setGuessRounds(_ => numOfRounds);
  };

  let configureNewGameHandler = () => {
    setUserNumber(_ => None);
    setGuessRounds(_ => 0);
  };

  let content =
    switch (userNumber) {
    | None => <Screens.StartGame onStartGame=startGameHandler />
    | Some(n) when guessRounds > 0 =>
      <Screens.GameOver
        roundsNumber=guessRounds
        userNumber=n
        onNewGame=configureNewGameHandler
      />
    | Some(n) => <Screens.Game userChoice=n onGameOver=gameOverHandler />
    };

  appLoading
    ? <Expo.AppLoading
        startAsync=fetchFonts
        onFinish={_ => setAppLoading(_ => false)}
        onError={e => Js.log(e)}>
        <Text> "Hi"->React.string </Text>
      </Expo.AppLoading>
    : <>
        <StatusBar barStyle=`darkContent />
        <View style={Style.style(~flex=1., ())}>
          <Components.Header title="Guess a Number" />
          content
        </View>
      </>;
};