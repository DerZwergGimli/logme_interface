export function sensorstatus_toString(dir: E_SensorStatus): string {
  return E_SensorStatus[dir];
}

export interface I_LIVEDATA {
  chart_data: Array<number>;
  chart_timestamps: Array<number>;
}

export interface I_Sensor {
  live_data?: I_LIVEDATA;
  name: string;
  id: number;
  description: string;
  status: E_SensorStatus;
  pin_rx: number;
  pin_tx: number;
  baudrate: number;
  primary_address: number;
  secondary_address: number;
  data?: Data;
  dashboard_config_ids: Array<number[]> | undefined;
  history?: {
    day_24_kw: Array<number>;
    week_7_kw: Array<number>;
    month_30_kw: Array<number>;
  };
}

export interface Data {
  salve_info: SalveInfo;
  slave_data: SlaveDatum[];
}

export interface SalveInfo {
  'id:': number;
  'manufacturer:': string;
  'version:': number;
  'product_name:': string;
  'medium:': string;
  'access_number:': number;
  'status:': string;
  'signature:': string;
}

export interface SlaveDatum {
  id: number;
  function: Function;
  storage_number?: number;
  unit?: string;
  value: string;
  timestamp: Date;
}

export enum Function {
  InstantaneousValue = 'Instantaneous value',
  MoreRecordsFollow = 'More records follow',
}

export enum E_SensorStatus {
  DEFAULT = 0,
  ERROR = 1,
  IDLE = 2,
  READING = 3,
}
