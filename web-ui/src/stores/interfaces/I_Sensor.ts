export interface I_Sensor {
  id: number;
  name: string;
  description: string;
  count: number;
  power: number;
  chart_data: Array<number> | undefined;
  chart_timestamps: Array<number> | undefined;
  history: {
    day_24_kw: Array<number>;
    week_7_kw: Array<number>;
    month_30_kw: Array<number>;
  };
}
