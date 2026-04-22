import { hilog } from '@kit.PerformanceAnalysisKit';
import testNapi from 'libentry.so';
import {NodeContent} from '@kit.ArkUI';

@Entry
@Component
struct Index {
  private nodeContent: NodeContent = new NodeContent();

  aboutToAppear():void{
    testNapi.createNativeNode(this.nodeContent,"ygb");
    testNapi.copyAssets(getContext().resourceManager, '');
  }

  build() {
    Column() {
      Row() {
        Text('glmark2')
          .fontSize('24fp')
          .fontWeight(500)
          .margin({
            left: 24,
            top: 12
          })
      }
      .margin({ top: 24 })
      .width('100%')
      .height(56)

      // Make this container take most of the screen
      Stack({ alignContent: Alignment.Center }) {
        ContentSlot(this.nodeContent)
      }
      .width('100%')
      .layoutWeight(1)  // Take all available space
      .backgroundColor(0x22000000) // Optional: add a light background to see the container

      Row() {
        Button('Run')
          .fontSize('16fp')
          .fontWeight(500)
          .margin({ bottom: 24 })
          .onClick(() => {
            testNapi.run();
          })
          .width('53.6%')
          .height(40)
      }
      .width('100%')
      .justifyContent(FlexAlign.Center)
      .alignItems(VerticalAlign.Bottom)
      .height('auto')
    }
    .width('100%')
    .height('100%')
  }
}
