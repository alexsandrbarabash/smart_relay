import { Component, input } from '@angular/core';

@Component({
  selector: 'app-field',
  standalone: true,
  templateUrl: './field.html',
  styleUrl: './field.scss',
})
export class Field {
  readonly label = input.required<string>();
  readonly hint = input<string>('');
}
