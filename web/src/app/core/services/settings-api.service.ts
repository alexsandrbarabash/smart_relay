import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders, HttpParams } from '@angular/common/http';
import { firstValueFrom } from 'rxjs';
import { TelegramTokenCommand, WifiSettingsCommand } from '../models/settings.models';

@Injectable({ providedIn: 'root' })
export class SettingsApiService {
  constructor(private readonly http: HttpClient) {}

  async setupWifi(command: WifiSettingsCommand): Promise<WifiSettingsCommand> {
    const body = new HttpParams()
      .set('ssid', command.ssid?.trim() ?? '')
      .set('pass', command.pass ?? '');

    await firstValueFrom(
      this.http.post('/setup', body.toString(), {
        headers: new HttpHeaders({ 'Content-Type': 'application/x-www-form-urlencoded' }),
      }),
    );

    return {
      mode: 'station',
      ssid: command.ssid?.trim(),
      pass: command.pass ?? '',
    };
  }

  async setupAccessPoint(): Promise<WifiSettingsCommand> {
    await firstValueFrom(this.http.post('/setup-ap', null));
    return { mode: 'ap' };
  }

  async saveTelegramToken(command: TelegramTokenCommand): Promise<TelegramTokenCommand> {
    const nextCommand = { token: command.token.trim() };

    await firstValueFrom(
      this.http.post('/telegram', nextCommand, {
        headers: new HttpHeaders({ 'Content-Type': 'application/json' }),
      }),
    );

    return nextCommand;
  }

  async syncTime(time: number): Promise<void> {
    await firstValueFrom(
      this.http.post(
        '/sync-time',
        { time },
        {
          headers: new HttpHeaders({ 'Content-Type': 'application/json' }),
          responseType: 'text',
        },
      ),
    );
  }
}
