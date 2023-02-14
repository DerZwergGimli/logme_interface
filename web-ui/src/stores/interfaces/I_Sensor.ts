export function sensorstatus_toString(dir: E_SensorStatus): string {
  return E_SensorStatus[dir];
}

export interface I_LIVEDATA {
  chart_data: Array<number>;
  chart_timestamps: Array<number>;
}

export interface I_Sensor {
  name:                 string;
  id:                   number;
  description:          string;
  status:               number;
  pin_rx:               number;
  pin_tx:               number;
  baudrate:             number;
  primary_address:      number;
  secondary_address:    number;
  dashboard_config_ids: Array<number[]>;
  data:                 I_Data;
}

export interface I_Data {
  salve_info: I_SalveInfo;
  slave_data: I_SlaveData[];
}

export interface I_SalveInfo {
  id:            number;
  manufacturer:  string;
  version:       number;
  product_name:  string;
  medium:        string;
  access_number: number;
  status:        string;
  signature:     string;
}

export interface I_SlaveData {
  id:             number;
  function:       I_SalveData_Function;
  storage_number: number;
  unit:           string;
  quantity:       string;
  value:          number | string;
}

export enum I_SalveData_Function {
  InstantaneousValue = 'Instantaneous value',
  MaximumValue = 'Maximum value',
  MoreRecordsFollow = 'More records follow',
  ValueDuringErrorState = 'Value during error state',
}

export enum E_SensorStatus {
  DEFAULT = 0,
  ERROR = 1,
  IDLE = 2,
  READING = 3,
}
