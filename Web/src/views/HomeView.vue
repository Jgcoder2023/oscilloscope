<template>
  <div class="main">
    <div class="title">简易示波器自动化测量在线平台</div>
    <div class="box">
      <div class="config">
        <el-form  label-width="auto">
          <el-form-item style="align-items: center;" label="串口操作:">
            <el-switch @change="EditComState" v-model="setValue.ComState" class="switch" active-text="已连接" inactive-text="已断开" size="large" />
          </el-form-item>

          <el-divider class="divider" content-position="left">信号输入方式</el-divider>
          <el-form-item style="align-items: center;" label="信号耦合:">
            <el-radio-group v-model="setValue.SignalDcAc" @change="ModifyValue('SignalDcAc')">
              <el-radio value="0" border>交流</el-radio>
              <el-radio value="1" border>直流</el-radio>
            </el-radio-group>
          </el-form-item>
          <el-form-item class="divider" style="align-items: center;" label="信号衰减:" >
            <el-radio-group v-model="setValue.SignalDecay" @change="ModifyValue('SignalDecay')">
              <el-radio value="0" border>1/50</el-radio>
              <el-radio value="1" border>不衰减</el-radio>
            </el-radio-group>
          </el-form-item>

          <el-divider class="divider" content-position="left">PWM输出配置</el-divider>
          <el-form-item style="align-items: center;" label="PWM开关:">
            <el-switch v-model="setValue.PwmState" class="switch" size="large" @change="ModifyValue('PwmState')" />
          </el-form-item>
          <el-form-item style="align-items: center;" label="PWM频率:">
            <el-input-number v-model="setValue.PwmFreq" :step="10" @input="ModifyValue('Other')" />
          </el-form-item>
          <el-form-item style="align-items: center;" label="PWM占空比:">
            <el-input-number v-model="setValue.PwmDutyCycle" :max="100" :step="1" @input="ModifyValue('Other')" />
          </el-form-item>

          <el-divider class="divider" content-position="left">DAC输出配置</el-divider>
          <el-form-item style="align-items: center;" label="DAC开关:">
            <el-switch v-model="setValue.DacState" class="switch" size="large"  @change="ModifyValue('DacState')"/>
          </el-form-item>
          <el-form-item style="align-items: center;" label="DAC电压:">
            <el-input-number v-model="setValue.DacVoltage" :precision="2" :step="0.1" :max="3.3" @input="ModifyValue('Other')" />
          </el-form-item>
        </el-form>
      </div>

      <div class="chars">
        <div id="CHARS"></div>
        <div class="gauge">
          <div>
            <span class="voltage">采集电压:{{getValue.voltage}}V</span>
            <span v-show="getValue.freq >= 1000">采集频率:{{ getValue.freq/1000 }}KHz</span>
            <span v-show="getValue.freq < 1000">采集频率:{{ getValue.freq }}Hz</span>
          </div>
          <div class="button">
            <el-tooltip class="box-item" effect="light" content="放大显示轴" placement="top"><el-button @click="formatSp(1)" :icon="Plus" circle /></el-tooltip>
            <el-tooltip class="box-item" effect="light" content="缩小显示轴" placement="top"><el-button @click="formatSp(0)" :icon="Minus" circle /></el-tooltip>
            <el-tooltip class="box-item" effect="light" content="导出csv文件" placement="top"><el-button :disabled="IsOutCsv" @click="IsOutCsv = 1" :icon="Files" circle /></el-tooltip>
          </div>

        </div>
      </div>
    </div>



  <div class="code_image">
    <el-divider content-position="left">* 单片机与网页通信协议说明 *</el-divider>
    <el-image alt="示波器数据传输格式示例www.daodaoyouli.com" src="/code.jpg" lazy />
  </div>


  </div>
</template>


<script setup lang="ts">
// https://zhuanlan.zhihu.com/p/456427866  参考文档
import { ElMessage } from 'element-plus'
import {onUnmounted, reactive, ref,onMounted} from "vue";
import { App,Leafer, Line, Point } from 'leafer-ui'
import { Minus,Plus,Files} from '@element-plus/icons-vue'

const setValue = reactive({
  ComState:false,
  switcValue:0,
  SignalDcAc:"0",
  SignalDecay:"0",
  PwmState:true,
  PwmFreq:1000,
  PwmDutyCycle:50,
  DacState:true,
  DacVoltage:1.65,
});
const getValue = reactive({
  freq:1000,
  voltage:3.2,
});

let reader = ref<any>() // 注册一个读取器
let writer = ref<any>() // 注册一个写操作
let port = ref<any>() // 注册一个串口

if ("serial" in navigator) {
  console.log("可以使用串口");
} else {
  ElMessage.error('该浏览器不知道串口,建议使用Chrome 或者 Edge 浏览器')
}

// 修改设置的参数下发
const ModifyValue = (name:string)=>{
  switch(name){
    case "PwmState":
      setValue.switcValue = setValue.PwmState ? (setValue.switcValue | 0b0001) : (setValue.switcValue & 0b1110);
    break
    case "DacState":
      setValue.switcValue = setValue.DacState ? (setValue.switcValue | 0b0010) : (setValue.switcValue & 0b1101);
    break
    case "SignalDcAc":
      setValue.switcValue = setValue.SignalDcAc == "1" ? (setValue.switcValue | 0b0100) : (setValue.switcValue & 0b1011);
    break;
    case "SignalDecay":
      setValue.switcValue = setValue.SignalDecay == "1" ? (setValue.switcValue | 0b1000) : (setValue.switcValue & 0b0111);
    break;
  }
  SerialWrite();
}



//  修改串口状态
const EditComState = async() => {
  if(setValue.ComState){
    SerialOpen().catch(res=>{
      console.log("打开失败");
      setValue.ComState = false;
      ElMessage.error('串口打开失败')
    });
  }else{
    SerialClose();
  }
}

// 打开串口
const SerialOpen = async() =>{
    // 提示用户选择一个串口
    port.value = await (navigator as any).serial.requestPort()
    // 等待串口打开
    await port.value.open({ baudRate: 115200 });
    ElMessage({type:'success', message:'串口连接成功!'})
    // 侦听串口数据
    await SerialRead()
}
// 关闭串口
const SerialClose = async() => {
  await reader.value.cancel()
    ElMessage({type:'success', message:'串口已关闭!'})
}



// 读取串口数据
const SerialRead = async () => {
  // 侦听串口传出的数据
  while (port.value.readable) {
    // 从串口读取数据
    let timer: any; // 计时器
    let u8arrValue: any;
    let arrTemp: Uint8Array[] = [];
    reader.value = port.value.readable.getReader();
    try {
      // 监听来自串行设备的数据
      while (true) {
        const { value, done } = await reader.value.read();
        if (done) {break;}
        clearTimeout(timer)
        u8arrValue = new Uint8Array([...arrTemp, ...value])
        arrTemp = u8arrValue
        timer = setTimeout(()=>{

          // console.log(u8arrValue);
          analysisData(u8arrValue);

          u8arrValue=[];
          arrTemp=[];
        },50)
      }
    } catch (error) {
      // TODO: 处理非致命的读错误。
      console.log(error);
    } finally {
      // 允许稍后关闭串口
      reader.value.releaseLock();
    }
    await port.value.close()
  }
}


// 写入串口数据
const SerialWrite = async () => {
  if(!setValue.ComState){
    ElMessage.error('未连接串口')
  }
  writer.value = await port.value.writable.getWriter();
  // let data = new Uint8Array("123456".split("").map((s) => s.charCodeAt(0)))   //发送字符串可以使用
  let data = new Uint8Array([0x5B,setValue.switcValue,(setValue.PwmFreq>>8)&0x00FF,(setValue.PwmFreq)&0x00FF,((setValue.DacVoltage*100)>>8)&0x00FF,(setValue.DacVoltage*100)&0x00FF,setValue.PwmDutyCycle,0x5D])
  console.log(data);
  await writer.value.write(data)
  // 允许稍后关闭串口。
  writer.value.releaseLock()
}

// 解析数据
const analysisData = (u8arr: Uint8Array)=>{
  // 判断总长度
  if(u8arr.length != 612){
    ElMessage.error('数据长度错误')
  }
  // 验证帧头帧尾
  if(u8arr[0] != 0x5B || u8arr[611] != 0x5D){
    ElMessage.error('帧头或帧尾错误');
  }

  setValue.switcValue = u8arr[1];
  // 说明全部通过检验
  if(u8arr[1] & 0x01){   //PWM开关状态
    setValue.PwmState = true;
  }else{
    setValue.PwmState = false;
  }

  if(u8arr[1] & 0x02){   //DAC开关状态
    setValue.DacState = true;
  }else{
    setValue.DacState = false;
  }

  if(u8arr[1] & 0x04){   //交直流耦合
    setValue.SignalDcAc = "1";
  }else{
    setValue.SignalDcAc = "0";
  }

  if(u8arr[1] & 0x08){   //是否衰减
    setValue.SignalDecay = "1";
  }else{
    setValue.SignalDecay = "0";
  }

  setValue.PwmDutyCycle = u8arr[610];  //获取PWM占空比

  // console.log(u8arr);
  // 将u8arr转为u16arr
  let u16Array = new Uint16Array(u8arr.buffer);

  exportCsv(u16Array); //导出csv

  // console.log(u16Array);

  getValue.voltage = u16Array[1] / 100; // 获取峰峰值
  getValue.freq = u16Array[2];
  setValue.PwmFreq = u16Array[3];
  setValue.DacVoltage = u16Array[4] / 1000;

  //获取中间值
  let median:number = getValue.voltage/2;
  //放大倍数，需要确定放大之后的区间，我将波形固定显示在（20~400中），(400-20)/2=190
  let gainFactor = 190/median/2;


  let tempValue:number=0;
  let preTempValue:number=0;
  // drawOscilloscope(u16Array.slice(5,305));
  for (let i = 5; i < 305; i++) {
    if(getValue.voltage > 0.35){
      tempValue = u16Array[i]/100;
      if(tempValue >= median){
        tempValue = Math.floor(240 - (tempValue - median)*gainFactor);
      }else{
        tempValue = Math.floor(240 + (median - tempValue)*gainFactor);
      }
    }else{
      // 说明没有波形
      tempValue = 400;
    }
    if(tempValue > 420 || tempValue < 20){
      tempValue = preTempValue;
    }
    preTempValue = tempValue;
    drawOscilloscope(tempValue);
  }
}



// 波形显示的宽高
let width = 800;
let height = 480;
onMounted(()=>{
  const leaferApp  = new App({
    view:'CHARS', 
    width: width, 
    height: height,
  });
  leaferApp.sky = leaferApp.addLeafer({ 
    type: 'draw',
    // wheel: { zoomMode: true },
    // zoom: { min: 0.1, max: 20 }, //设置缩放范围
  })

// 正弦波示例
const line = new Line({
    points: waveData,
    curve: false,
    strokeWidth: 2,
    stroke: "#67c23a"
})

function animate() {
    requestAnimationFrame(animate);
    leaferApp.sky.add(line)
}
  animate()
});

// 处理X轴的数据坐标
let waveData:number[] = [];  // 存储波形需要的数组
let x:number = 0; //X轴的坐标
// 绘制示波器的函数
function drawOscilloscope(value:number) {
    if (x > width) {
      for (let i = 1; i < waveData.length; i += 2) {
        waveData[i] = waveData[i + 2]
      }
      waveData[waveData.length - 1] = value;
    } else {
      waveData.push(x)
      waveData.push(value)
      x++;
    }
    // console.log(waveData);
}

// 定义以及处理x轴的间隙值
let sp=1;
const formatSp = (val: number) => {
  if(val >0){
    sp += 0.2;
  }else{
    sp -= 0.2;
  }
  if(sp < 1){
    ElMessage.error('缩放已经到极限了')
    sp=1;
  }
  if(sp > 8){
    ElMessage.error('缩放已经到极限了')
    sp = 8;
  }
  controlX();
}
// 开始调节X轴的间隔
const controlX = ()=>{
  let co:number = 0;
  for (let i = 0; i < waveData.length; i+=2) {
    if(i>0){
      waveData[i] = waveData[i-2] + sp;
    }
  }
}




//导出csv
const IsOutCsv = ref(0);
const exportCsv = (data:any) => {
  if(IsOutCsv.value == 0){
    return;
  }

  // 导出数据的临时遍历
  let outData = [];
  for (let i = 5; i < 305; i++) {
    outData.push({"no":i-4,"value":data[i]});  
  }

 
  const header:any = { 'no': '编号', 'value': 'ADC采样值' };
  let csv = '';
  for (let key in header) {
      csv += header[key] + ',';
  }
  csv = csv.substr(0, csv.length - 1) + '\r\n';
  for (let i in outData) {
    for (let key in header) {
      csv += outData[i][key] + ',';
    }
    csv = csv.substr(0, csv.length - 1) + '\r\n';
  }
  //定义文件内容，类型必须为Blob 否则createObjectURL会报错
   let content = new Blob(['\uFEFF' + csv]);
  //生成url对象
   let urlObject = window.URL || window.webkitURL || window;
   let url = urlObject.createObjectURL(content);
  //生成<a></a>DOM元素
   let el = document.createElement("a");
   //链接赋值
   el.href = url;
   el.download = "文件导出.csv";
   //必须点击否则不会下载
   el.click();
  //移除链接释放资源
  urlObject.revokeObjectURL(url);

  IsOutCsv.value = 0; //导出结束
}


</script>


<style lang="scss" scoped>
.main{
  text-align: center;
  .title{
    width: 100vw;
    height: auto;
    font-size: 26px;
    font-weight: 700;
    text-align: center;
    margin-top: 40px;
  }
  .box{
    margin: 40px auto auto auto;
    padding: 0 20px;
    display: flex;
    justify-content: center;
    .config{
      border: dashed 1px #000000;
      border-radius: 5px;
      width: 400px;
      max-width: 600px;
      min-width: 400px;
      padding: 5px;
      .switch{
        --el-switch-on-color: #13ce66;
        --el-switch-off-color: #ff4949;
      }
      .divider{
        width: 400px;
      }
    }
    .chars{
      margin-left: 10px;
      background-color: #000000;
      width: 800px;
      height: 520px;
      border: dashed 1px #000000;
      padding: 5px;
      border-radius: 5px;

      .gauge{
        display: flex;
        justify-content: space-between;
          div{
            font-size: 22px;
            color: #67c23a;
            .voltage{
              margin-left: 10px;
              margin-right: 20px;
          }
        }

        .button{
          margin-right: 10px;
        }
      }
    }
  }
  .code_image{
    margin: 40px auto;
    width: 1220px;
    border: dashed 1px #000000;
    border-radius: 5px;
    padding: 5px;
  }
}
</style>
