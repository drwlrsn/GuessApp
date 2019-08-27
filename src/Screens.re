open ReactNative;
open Components;

module StartGame = {
  let styles =
    Style.(
      StyleSheet.create({
        "screen": style(~flex=1., ~padding=10.->dp, ~alignItems=`center, ()),
        "title": style(~fontSize=20., ~marginVertical=10.->dp, ()),
        "inputContainer":
          style(~width=300.->dp, ~maxWidth=80.->pct, ~alignItems=`center, ()),
        "buttonContainer":
          style(
            ~flexDirection=`row,
            ~width=100.->pct,
            ~justifyContent=`spaceBetween,
            ~paddingHorizontal=15.->dp,
            (),
          ),
        "button": style(~width=100.->dp, ()),
        "input": style(~width=50.->dp, ~textAlign=`center, ()),
        "summaryContainer":
          style(~marginTop=20.->dp, ~alignItems=`center, ()),
      })
    );

  module ConfirmedOutput = {
    [@react.component]
    let make = (~show, ~selectedNumber: option(int), ~onStartGame) =>
      show
        ? <Card style=styles##summaryContainer>
            <BodyText> "You selected"->React.string </BodyText>
            <NumberContainer>
              {Belt.Option.mapWithDefault(selectedNumber, ""->React.string, v =>
                 v |> string_of_int |> React.string
               )}
            </NumberContainer>
            <MainButton onPress={_ => onStartGame(selectedNumber)}>
              "START GAME"->React.string
            </MainButton>
          </Card>
        : React.null;
  };

  [@react.component]
  let make = (~onStartGame) => {
    let (enteredValue, setEnteredValue) = React.useState(() => "");
    let (confirmed, setConfirmed) = React.useState(() => false);
    let (selectedNumber, setSelectedNumber) = React.useState(_ => None);
    let numberInputHandler = v =>
      setEnteredValue(_ =>
        Js.String.replaceByRe([%bs.re "/[^0-9]/g"], "", v)
      );

    let resetInputHandler = _ => {
      setEnteredValue(_ => "");
      setConfirmed(_ => false);
    };

    let confirmInputHandler = _ =>
      switch (Js.Float.fromString(enteredValue)) {
      | v when v->Js.Float.isNaN || v < 1. || v > 99. =>
        Alert.alert(
          ~title="Invalid Number!",
          ~message="Number has to be a number between 1 and 99",
          ~buttons=[|
            Alert.button(
              ~text="Okay",
              ~style=`destructive,
              ~onPress=resetInputHandler,
              (),
            ),
          |],
          (),
        )
      | _ =>
        setConfirmed(_ => true);
        setEnteredValue(_ => "");
        setSelectedNumber(_ => enteredValue->int_of_string->Some);
        Keyboard.dismiss();
      };

    <TouchableWithoutFeedback onPress={_ => Keyboard.dismiss()}>
      <View style=styles##screen>
        <TitleText style=styles##title>
          "Start a New Game!"->React.string
        </TitleText>
        <Card style=styles##inputContainer>
          <BodyText> "Select a Number"->React.string </BodyText>
          <Input
            style={styles##input}
            blurOnSubmit=true
            autoCapitalize=`none
            autoCorrect=false
            keyboardType=`numeric
            maxLength=2
            onChangeText=numberInputHandler
            value=enteredValue
          />
          <View style={styles##buttonContainer}>
            <View style={styles##button}>
              <Button
                title="Reset"
                onPress=resetInputHandler
                color=Constants.Colors.accent
              />
            </View>
            <View style={styles##button}>
              <Button
                title="Confirm"
                onPress=confirmInputHandler
                color=Constants.Colors.primary
              />
            </View>
          </View>
        </Card>
        <ConfirmedOutput show=confirmed selectedNumber onStartGame />
      </View>
    </TouchableWithoutFeedback>;
  };
};

module Game = {
  type direction =
    | Lower(int)
    | Higher(int);

  Random.init(Js.Date.now()->int_of_float);

  let rec generateRandomBetween = (min, max, ~exclude=?, ()) => {
    let n = Random.int(max - min) + min;

    switch (exclude) {
    | Some(v) when n == v => generateRandomBetween(min, max, ~exclude=v, ())
    | Some(_) => generateRandomBetween(min, max, ())
    | None => n
    };
  };

  let styles =
    Style.(
      StyleSheet.create({
        "screen": style(~flex=1., ~padding=10.->dp, ~alignItems=`center, ()),
        "buttonContainer":
          style(
            ~flexDirection=`row,
            ~justifyContent=`spaceAround,
            ~marginVertical=20.->dp,
            ~width=400.->dp,
            ~maxWidth=90.->pct,
            (),
          ),
        "listItem":
          style(
            ~borderColor="#ccc",
            ~padding=15.->dp,
            ~marginVertical=10.->dp,
            ~backgroundColor="white",
            ~borderWidth=1.,
            ~flexDirection=`row,
            ~justifyContent=`spaceAround,
            ~width=100.->pct,
            (),
          ),
        "listContainer": style(~width=60.->pct, ~flex=1., ()),
        "list":
          style(
            // ~alignItems=`center,
            ~justifyContent=`flexEnd,
            ~flexGrow=1.,
            // ~width=100.->pct,
            (),
          ),
      })
    );

  let renderItem = (listLength, itemData) =>
    <View style=styles##listItem>
      <Text>
        {("# " ++ (listLength - itemData##index)->string_of_int)->React.string}
      </Text>
      <Text> {itemData##item->string_of_int->React.string} </Text>
    </View>;

  [@react.component]
  let make = (~userChoice, ~onGameOver) => {
    let currentLow = React.useRef(() => 1);
    let currentHigh = React.useRef(() => 99);
    let initialGuess =
      generateRandomBetween(
        React.Ref.current(currentLow, ()),
        React.Ref.current(currentHigh, ()),
        ~exclude=userChoice,
        (),
      );
    let (currentGuess, setCurrentGuess) = React.useState(() => initialGuess);
    let (pastGuesses, setPastGuesses) = React.useState(() => [initialGuess]);
    React.useEffect3(
      () => {
        if (currentGuess == userChoice) {
          onGameOver(List.length(pastGuesses));
        };
        Some(() => ());
      },
      (currentGuess, userChoice, onGameOver),
    );

    let makeAlert =
      Alert.alert(
        ~title="Don't lie!",
        ~message="You know that this is wrong!",
        ~buttons=[|Alert.button(~text="Sorry!", ~style=`cancel, ())|],
      );

    let nextGuessHandler = direction => {
      switch (direction) {
      | Lower(guess) when guess < userChoice => makeAlert()
      | Higher(guess) when guess > userChoice => makeAlert()
      | Lower(guess) =>
        React.Ref.setCurrent(currentHigh, () => currentGuess);
        let nextGuess =
          generateRandomBetween(
            React.Ref.current(currentLow, ()),
            guess + 1,
            (),
          );
        setPastGuesses(s => [nextGuess, ...s]);
        setCurrentGuess(_ => nextGuess);
      | Higher(guess) =>
        React.Ref.setCurrent(currentLow, () => currentGuess + 1);
        let nextGuess =
          generateRandomBetween(
            guess,
            React.Ref.current(currentHigh, ()),
            (),
          );
        setPastGuesses(s => [nextGuess, ...s]);
        setCurrentGuess(_ => nextGuess);
      };
    };

    <View style=styles##screen>
      <Text style=Constants.Typography.style##body>
        "Opponents Guess"->React.string
      </Text>
      <NumberContainer>
        {currentGuess |> string_of_int |> React.string}
      </NumberContainer>
      <Card style=styles##buttonContainer>
        <MainButton onPress={_ => nextGuessHandler(Lower(currentGuess))}>
          <Icons.AntDesign name="minuscircleo" size=24. color="white" />
        </MainButton>
        <MainButton onPress={_ => nextGuessHandler(Higher(currentGuess))}>
          <Icons.AntDesign name="pluscircleo" size=24. color="white" />
        </MainButton>
      </Card>
      <View style=styles##listContainer>
        // <ScrollView contentContainerStyle=styles##list>
        //   {pastGuesses
        //    |> Array.of_list
        //    |> Array.mapi((idx, guess) =>
        //         <View key={guess->string_of_int} style=styles##listItem>
        //           <Text>
        //             {(
        //                "# "
        //                ++ List.length(pastGuesses)->(-)(idx)->string_of_int
        //              )
        //              ->React.string}
        //           </Text>
        //           <Text> {guess->string_of_int->React.string} </Text>
        //         </View>
        //       )
        //    |> React.array}
        // </ScrollView>

          <FlatList
            contentContainerStyle=styles##list
            data={pastGuesses->Array.of_list}
            renderItem={renderItem(List.length(pastGuesses))}
            keyExtractor={(item, _) => item->string_of_int}
          />
        </View>
    </View>;
  };
};

module GameOver = {
  let styles =
    Style.(
      StyleSheet.create({
        "screen":
          style(~flex=1., ~justifyContent=`center, ~alignItems=`center, ()),
        "image": style(~width=100.->pct, ~height=100.->pct, ()),
        "iamgeContainer":
          style(
            ~borderRadius=150.,
            ~borderWidth=3.,
            ~borderColor="black",
            ~width=300.->dp,
            ~height=300.->dp,
            ~overflow=`hidden,
            ~marginVertical=30.->dp,
            (),
          ),
        "highlight":
          style(
            ~color=Constants.Colors.primary,
            ~fontFamily="open-sans-bold",
            ~marginHorizontal=30.->dp,
            (),
          ),
        "resultContainer":
          style(~marginHorizontal=30.->dp, ~marginVertical=15.->dp, ()),
        "resultText": style(~textAlign=`center, ~fontSize=20., ()),
      })
    );

  [@react.component]
  let make = (~roundsNumber, ~userNumber, ~onNewGame) =>
    <View style=styles##screen>
      <TitleText> "The Game is Over"->React.string </TitleText>
      <View style=styles##iamgeContainer>
        // <Image
        //   style=styles##image
        //   source={Image.Source.fromRequired(
        //     Packager.require("../assets/success.png"),
        //   )}
        // />

          <Image
            style=styles##image
            source={Image.Source.fromUriSource(
              Image.uriSource(
                ~uri=
                  "https://proxy.duckduckgo.com/iu/?u=https%3A%2F%2Fwww.alpineascents.com%2Fwp-content%2Fuploads%2F2016%2F07%2Fmatterhorn1.jpg&f=1",
                (),
              ),
            )}
          />
        </View>
      <View style=styles##resultContainer>
        <BodyText style=styles##resultText>
          <Text> "Your phone needed "->React.string </Text>
          <Text style=styles##highlight>
            {roundsNumber->string_of_int->React.string}
          </Text>
          <Text> " rounds to guess the number "->React.string </Text>
          <Text style=styles##highlight>
            {userNumber->string_of_int->React.string}
          </Text>
        </BodyText>
      </View>
      <MainButton onPress={_ => onNewGame()}>
        "NEW GAME"->React.string
      </MainButton>
    </View>;
};