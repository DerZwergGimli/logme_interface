export interface Sensor {
  id: number;
  name: string;
  count: number;
  power: number;
  chart_data: Array<number>;
  chart_timestamps: Array<number>;
}
