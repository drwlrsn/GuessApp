module AntDesign = {
  [@bs.module "@expo/vector-icons/AntDesign"] [@react.component]
  external make:
    (~name: string, ~size: float=?, ~color: string=?) => React.element =
    "default";
};